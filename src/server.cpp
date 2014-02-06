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

using namespace std;

/* static */
Server* Server::sInstance = nullptr;

/* static */
Server&
Server :: getInstance()
{
    static volatile long protect = 0;

    if (sInstance == nullptr)
    {
        if (1 == atomic_inc(&protect))
        {
            // create the instance
            sInstance = new Server();
        }
        else
        {
            while (sInstance == nullptr)
                QThread::yieldCurrentThread();
        }
    }
    return *sInstance;
}

Server :: Server()
{
    err_t err = ERROR_SUCCESS;

    // init the logger...
    DOIF(err, Logger::init("./", "zfserv.log"));

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
    DOIF(err, Script::registerFunctions()); // register shared Lua functions
    DOIF(err, NpcTask::registerFunctions()); // register NPC's Lua functions
    DOIF(err, ItemTask::registerFunctions()); // register item's Lua functions

    // load DMap files
    MapManager& mgr = MapManager::getInstance();
    DOIF(err, mgr.loadData());

    // load database
    DOIF(err, db.loadAllMaps());
    DOIF(err, db.loadAllItems());
    DOIF(err, db.loadAllNPCs());
    DOIF(err, db.loadAllTasks());
    DOIF(err, db.loadAllMonsters());
    DOIF(err, db.loadAllGenerators());

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

    ASSERT(err == ERROR_SUCCESS);
}

Server :: ~Server()
{

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
