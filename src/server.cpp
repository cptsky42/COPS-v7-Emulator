/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "server.h"

#include "client.h"
#include "player.h"

#include "networkclient.h"
#include "msg.h"
#include "msgloginchallenges.h"

#include "mapmanager.h"
#include "database.h"
#include "world.h"

#include "script.h"
#include "npctask.h"
#include "itemtask.h"

#include "inifile.h"

extern "C" {
#include "bigint.h"
}

#include <stdlib.h> // srandom()
#include <time.h>

#ifdef _WIN32
#define NOMINMAX // want std::min() & std::max() defined...
#include <windows.h>

#ifdef _MSC_VER // Visual Studio will complain for linking...
#pragma comment(lib,"version.lib") //
#endif // _MSC_VER

#else
#include <sys/utsname.h> // uname()
#endif // _WIN32

using namespace std;

/* static */
const Server&
Server :: getInstance()
{
    Server* instance = dynamic_cast<Server*>(QCoreApplication::instance());

    ASSERT(instance != nullptr);
    return *instance;
}

/* static */
const char*
Server :: getServerInfo()
{
    static volatile long protect = 0;
    static char* SERVER_INFO = nullptr;

    if (SERVER_INFO == nullptr)
    {
        if (1 == atomic_inc(&protect))
        {
            #ifndef _WIN32
            struct utsname info;
            if (uname(&info) != -1)
            {
                size_t len =
                        strlen(info.sysname) + 1 +
                        strlen(info.nodename) + 1 +
                        strlen(info.release) + 1 +
                        strlen(info.version) + 1 +
                        strlen(info.machine) + 1;

                char* str = new char[len];
                snprintf(str, len, "%s %s %s %s %s",
                         info.sysname, info.nodename, info.release, info.version, info.machine);

                SERVER_INFO = str;
            }
            else
            {
                SERVER_INFO = new char[1];
                SERVER_INFO[0] = '\0';
            }
            #else
            char nodename[32] = { '\0' }; // max len = 15
            DWORD bufLen = (DWORD)sizeof(nodename);
            GetComputerNameA(nodename, &bufLen);

            char release[128] = { '\0' };
            if (GetFileVersionInfoA("kernel32.dll", 0, sizeof(release), release) != 0)
            {
                VS_FIXEDFILEINFO* fileInfo;
                UINT bufLen = 0;

                if (VerQueryValueA(release, "\\", (LPVOID*)&fileInfo, (PUINT)&bufLen))
                {
                    snprintf(release, sizeof(release), "%d.%d.%d.%d",
                             HIWORD(fileInfo->dwFileVersionMS), LOWORD(fileInfo->dwFileVersionMS),
                             HIWORD(fileInfo->dwFileVersionLS), LOWORD(fileInfo->dwFileVersionLS));
                }
            }

            char version[256] = { '\0' };
            snprintf(version, sizeof(version), "NT Kernel Version %s",
                     release);

            bool is64bit = GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process") != NULL;
            char machine[16] = { '\0' };
            snprintf(machine, sizeof(machine), "%s",
                     is64bit ? "x86_64" : "x86");

            size_t len =
                    strlen("Windows") + 1 +
                    strlen(nodename) + 1 +
                    strlen(release) + 1 +
                    strlen(version) + 1 +
                    strlen(machine) + 1;

            char* str = new char[len];
            snprintf(str, len, "Windows %s %s %s %s",
                     nodename, release, version, machine);

            SERVER_INFO = str;
            #endif // ! _WIN32
        }
        else
        {
            while (SERVER_INFO == nullptr)
                QThread::yieldCurrentThread();
        }
    }

    return SERVER_INFO;
}

Server :: Server(int argc, char* argv[])
    : QCoreApplication(argc, argv)
{
    #ifdef _WIN32
    SetConsoleTitleA(STR_PROGRAM_TITLE);
    #endif // _WIN32

    fprintf(stdout, "%s\n", STR_CREATOR_INFO);
    fprintf(stdout, "%s\n", STR_BUILD_INFO);
    fprintf(stdout, "\n");

    // init the logger...
    Logger::init("./", "zfserv.log");
    LOG(INFO, "============== Server - Begin ==============");

    // Initialize the BigInt package
    LOG(INFO, "Initializing the BigInt package...");
    bi_initialize();
    srand(time(NULL)); // for generatePrime() call

    // connect signals & slots
    connect(this, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));
}

int
Server :: exec()
{
    err_t err = ERROR_SUCCESS;

    // parse the config file...
    IniFile settings;
    DOIF(err, settings.open("./settings.cfg"));

    mServerName = settings.readString("COPS_EMULATOR/NAME", "COPS");
    mServerIP = settings.readString("COPS_EMULATOR/SERVER_IP", "127.0.0.1");

    string sql_host = settings.readString("COPS_EMULATOR/SQL_HOST", "localhost");
    string sql_db = settings.readString("COPS_EMULATOR/SQL_DB", "zfserver");
    string sql_user = settings.readString("COPS_EMULATOR/SQL_USER", "root");
    string sql_pwd = settings.readString("COPS_EMULATOR/SQL_PWD", "");

    // try to connect to the database...
    Database& db = const_cast<Database&>(Database::getInstance());
    if (!db.connect(sql_host.c_str(), sql_db.c_str(),
                    sql_user.c_str(), sql_pwd.c_str()))
    {
        fprintf(stderr, "Failed to connect to the database...\n");
        LOG(ERROR, "Failed to connect to the database...");
        err = ERROR_INVALID_PASSWORD;
    }

    // load Lua VM
    DOIF(err, Script::registerFunctions()); // register Lua functions

    // load DMap files
    MapManager& mgr = MapManager::getInstance();
    DOIF(err, mgr.loadData());

    // load database
    DOIF(err, db.loadAllMaps());
    DOIF(err, db.loadAllTasks());
    DOIF(err, db.loadAllItems());
    DOIF(err, db.loadAllNPCs());
    DOIF(err, db.loadAllMonsters());
    DOIF(err, db.loadAllGenerators());
    DOIF(err, db.loadAllMagics());

    fprintf(stdout, "\n");

    mAccServer.listen(Server::ACCSERVER_PORT);
    mAccServer.onConnect = &Server::connectionHandler;
    mAccServer.onReceive = &Server::receiveHandler;
    mAccServer.onDisconnect = &Server::disconnectionHandler;
    mAccServer.accept();
    fprintf(stdout, "AccServer listening on port %u...\n", Server::ACCSERVER_PORT);

    mMsgServer.listen(Server::MSGSERVER_PORT);
    mMsgServer.onConnect = &Server::connectionHandler;
    mMsgServer.onReceive = &Server::receiveHandler;
    mMsgServer.onDisconnect = &Server::disconnectionHandler;
    mMsgServer.accept();
    fprintf(stdout, "MsgServer listening on port %u...\n", Server::MSGSERVER_PORT);

    fprintf(stdout, "Waiting for connections...\n");

    ASSERT_ERR(err == ERROR_SUCCESS, err);
    return QCoreApplication::exec();
}

Server :: ~Server()
{
    // Terminate the BigInt package
    LOG(INFO, "Destroying the BigInt package...");
    bi_terminate();
}

void
Server :: aboutToQuit()
{
    // TODO save everything...
}

/* static */
void
Server :: connectionHandler(NetworkClient* aClient)
{
    LOG(DBG, "Incoming connection... %p\n", aClient);
    if (aClient != nullptr)
    {
        uint16_t port = ((TcpServer*)aClient->getServer())->getPort();

        Client* client = nullptr;
        switch (port)
        {
            case Server::ACCSERVER_PORT:
                {
                    client = new Client(aClient, ICipher::TQ_CIPHER);
                    client->setStatus(Client::NOT_AUTHENTICATED);

                    break;
                }
            case Server::MSGSERVER_PORT:
                {
                    client = new Client(aClient, ICipher::BLOWFISH);
                    client->setStatus(Client::KEY_EXCHANGE);

                    client->generateExchangeRequest();
                    break;
                }
            default:
                ASSERT(false); // received an unknown request
                break;
        }

        aClient->setOwner(client);
    }
}

/* static */
void
Server :: receiveHandler(NetworkClient* aClient, uint8_t* aBuf, size_t aLen)
{
    if (aClient != nullptr && aBuf != nullptr)
    {
        if (aLen < sizeof(Msg::Header))
            return;

        uint8_t* received = new uint8_t[aLen];
        memcpy(received, aBuf, aLen);

        // TODO? clean this line and add some checks
        Client* client = (Client*)aClient->getOwner();
        switch (client->getStatus())
        {
            case Client::KEY_EXCHANGE:
                {
                    // Decrypt the padding and the size of the MsgLoginChallengeS
                    // than, decrypt the rest of the MsgLoginChallengeS based on the read size
                    // than, handle the exchange response
                    // than, update the received buffer with the remaining data to be decrypted with
                    // the updated cipher...
                    ICipher& cipher = client->getCipher();

                    uint8_t* ptr = received;
                    size_t size = 0;

                    cipher.decrypt(ptr, MsgLoginChallengeS::PADDING_LEN + sizeof(int32_t));
                    ptr += MsgLoginChallengeS::PADDING_LEN; // Padding
                    #if BYTE_ORDER == BIG_ENDIAN
                    size = bswap32(*((int32_t*)ptr));
                    #else
                    size = *((int32_t*)ptr);
                    #endif // BYTE_ORDER == BIG_ENDIAN
                    cipher.decrypt(ptr + sizeof(int32_t), size - sizeof(int32_t)); // skip the Size, but decrypt the whole msg

                    size_t len = MsgLoginChallengeS::PADDING_LEN + size;
                    uint8_t* tmp = new uint8_t[len];
                    memcpy(tmp, received, len);

                    client->handleExchangeResponse(&tmp, len);
                    SAFE_DELETE_ARRAY(tmp);

                    ptr += size; // all the MsgLoginChallengeS data
                    aLen -= len;
                    memcpy(received, ptr, aLen);
                }
            default:
                {
                    if (aLen < sizeof(Msg::Header))
                        return;

                    client->getCipher().decrypt(received, aLen);

                    size_t size = 0;
                    for (size_t i = 0; i < aLen; i += size)
                    {
                        #if BYTE_ORDER == BIG_ENDIAN
                        size = bswap16(((Msg::Header*)(received + i))->Length);
                        #else
                        size = ((Msg::Header*)(received + i))->Length;
                        #endif

                        if (size == 20820) // HACK! TQ to uint16_t...
                        {
                            static const size_t SEAL_LEN = strlen("TQClient");
                            size = SEAL_LEN;

                            continue;
                        }

                        ASSERT(size <= 1024); // invalid msg size...
                        if (size < aLen)
                        {
                            uint8_t* packet = new uint8_t[size];
                            memcpy(packet, received + i, size);

                            #if BYTE_ORDER == BIG_ENDIAN
                            Msg::Header* header = (Msg::Header*)packet;
                            header->Length = bswap16(header->Length);
                            header->Type = bswap16(header->Type);
                            #endif

                            Msg* msg = nullptr;
                            Msg::create(&msg, &packet, size);
                            msg->process(client);

                            SAFE_DELETE(msg);
                            SAFE_DELETE_ARRAY(packet);
                        }
                        else
                        {
                            #if BYTE_ORDER == BIG_ENDIAN
                            Msg::Header* header = (Msg::Header*)received;
                            header->Length = bswap16(header->Length);
                            header->Type = bswap16(header->Type);
                            #endif

                            Msg* msg = nullptr;
                            Msg::create(&msg, &received, size);
                            msg->process(client);

                            SAFE_DELETE(msg);
                        }
                    }

                    break;
                }
        }

        SAFE_DELETE_ARRAY(received);
    }
}

/* static */
void
Server :: disconnectionHandler(NetworkClient* aClient)
{
    LOG(DBG, "Incoming disconnection... %p\n", aClient);
    if (aClient != nullptr)
    {
        // TODO? clean this line and add some checks
        Client* client = (Client*)aClient->getOwner();
        client->save();

        Player* player = client->getPlayer();
        if (player != nullptr)
        {
            World& world = World::getInstance();

            world.removePlayer(*player);
            player->leaveMap();
        }

        SAFE_DELETE(client);
    }
}
