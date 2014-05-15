/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgaccount.h"
#include "client.h"
#include "server.h"
#include "database.h"
#include "msgconnectex.h"
#include <string>
#include <string.h>
#include "rc5.h"

using namespace std;

MsgAccount :: MsgAccount(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgAccount :: ~MsgAccount()
{

}

void
MsgAccount :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);

    static const Database& db = Database::getInstance(); // singleton

    Client& client = *aClient;
    client.setAccount(mInfo->Account);

    uint8_t seed[RC5::KEY_SIZE] =
            { 0x3C, 0xDC, 0xFE, 0xE8, 0xC4, 0x54, 0xD6, 0x7E, 0x16, 0xA6, 0xF8, 0x1A, 0xE8, 0xD0, 0x38, 0xBE };

    RC5 cipher;
    cipher.generateKey(seed);
    cipher.decrypt((uint8_t*)mInfo->Password, sizeof(mInfo->Password));

    if (isValidString(mInfo->Account) && isValidString(mInfo->Password))
    {
        if (IS_SUCCESS(db.authenticate(client, mInfo->Account, mInfo->Password)))
        {
            fprintf(stdout, "Connection of %s on %s...\n",
                    mInfo->Account, mInfo->Server);

            int32_t token = random(10000, INT32_MAX);

            MsgConnectEx msg(client.getAccountID(), token,
                             Server::getServerIP(), Server::MSGSERVER_PORT);
            client.send(&msg);
        }
        else
        {
            MsgConnectEx msg(MsgConnectEx::ERROR_ID_INVALID_PWD);
            client.send(&msg);
        }
    }
    else
    {
        client.disconnect();
    }
}
