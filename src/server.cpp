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
#include "networkclient.h"
#include "msg.h"
#include "mapmanager.h"
#include "database.h"
#include "script.h"
#include "npctask.h"
#include "itemtask.h"
#include "inifile.h"

using namespace std;

/* static */
const uint16_t Server::ACCSERVER_PORT = 9958;
const uint16_t Server::MSGSERVER_PORT = 5816;

/* static */
Server* Server::sInstance = nullptr;

/* static */
Server&
Server :: getInstance()
{
    // TODO? Thread-safe
    if (sInstance == nullptr)
    {
        sInstance = new Server();
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

    string name = settings.readString("COPS_EMULATOR/NAME", "COPS"); // TODO it
    mServerIP = settings.readString("COPS_EMULATOR/SERVER_IP", "127.0.0.1");

    string sql_host = settings.readString("COPS_EMULATOR/SQL_HOST", "localhost");
    string sql_db = settings.readString("COPS_EMULATOR/SQL_DB", "zfserver");
    string sql_user = settings.readString("COPS_EMULATOR/SQL_USER", "root");
    string sql_pwd = settings.readString("COPS_EMULATOR/SQL_PWD", "");

    // try to connect to the database...
    Database& db = Database::getInstance();
    if (!db.connect(sql_host.c_str(), sql_db.c_str(),
                    sql_user.c_str(), sql_pwd.c_str()))
    {
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

    fprintf(stdout, "\n");

    mAccServer.listen(ACCSERVER_PORT);
    mAccServer.onConnect = &Server::connectionHandler;
    mAccServer.onReceive = &Server::receiveHandler;
    mAccServer.onDisconnect = &Server::disconnectionHandler;
    mAccServer.accept();
    fprintf(stdout, "AccServer listening on port %u...\n", ACCSERVER_PORT);

    mMsgServer.listen(MSGSERVER_PORT);
    mMsgServer.onConnect = &Server::connectionHandler;
    mMsgServer.onReceive = &Server::receiveHandler;
    mMsgServer.onDisconnect = &Server::disconnectionHandler;
    mMsgServer.accept();
    fprintf(stdout, "MsgServer listening on port %u...\n", MSGSERVER_PORT);

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
            case ACCSERVER_PORT:
                {
                    client = new Client(aClient, ICipher::TQ_CIPHER);
                    client->setStatus(Client::NOT_AUTHENTICATED);

                    break;
                }
            case MSGSERVER_PORT:
                {
                    client = new Client(aClient, ICipher::BLOWFISH);
                    client->setStatus(Client::KEY_EXCHANGE);

                    // TODO
                    //client->generateExchangeRequest();

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
        client->getCipher().decrypt(received, aLen);

        switch (client->getStatus())
        {
            case Client::KEY_EXCHANGE:
                {
                    // TODO
                    //client->handleExchangeResponse(received, aLen);
                    break;
                }
            default:
                {
                    size_t size = 0;
                    for (size_t i = 0; i < aLen; i += size)
                    {
                        #if BYTE_ORDER == BIG_ENDIAN
                        size = bswap16(((Msg::Header*)(received + i))->Length);
                        #else
                        size = ((Msg::Header*)(received + i))->Length;
                        #endif

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

        SAFE_DELETE(client);
    }
}
