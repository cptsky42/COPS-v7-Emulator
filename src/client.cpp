/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "client.h"
#include "networkclient.h"
#include "msg.h"
#include "player.h"
#include "database.h"
#include "world.h"
#include <stdlib.h>

Client :: Client(NetworkClient* aSocket)
{
    ASSERT(aSocket != nullptr);

    mSocket = aSocket;
    mCipher.generateIV(0x0705FD1F, 0x1B7A313F);

    mStatus = Client::NOT_AUTHENTICATED;

    mAccountID = -1;
    mAccLvl = 0;
    mFlags = 0;

    mPlayer = nullptr;
}

Client :: ~Client()
{
    mSocket = nullptr; // Qt handle the memory

    if (mPlayer != nullptr)
    {
        World& world = World::getInstance();
        world.removePlayer(*mPlayer);

        SAFE_DELETE(mPlayer);
    }
}

void
Client :: save()
{
    // TODO
    printf("Calling save for %p ... %s\n",
           this, mPlayer != nullptr ? mPlayer->getName() : "");

    err_t err = ERROR_SUCCESS;
    int tries = 0;

    if (mPlayer != nullptr)
    {
        Database& db = Database::getInstance();

        do
        {
            err = db.savePlayer(*this);
            ++tries;

            if (!IS_SUCCESS(err))
            {
                LOG(WARN, "Failed to save player %s for %p. Trying again.",
                    mPlayer->getName(), this);
            }
        }
        while (err != ERROR_SUCCESS && tries < 3);

        if (!IS_SUCCESS(err))
        {
            LOG(CRIT, "Failed to save player %s for %p.",
                mPlayer->getName(), this);
        }
    }
}

void
Client :: send(Msg* aMsg)
{
    ASSERT(aMsg != nullptr);

    uint8_t* data = new uint8_t[aMsg->getLength()];
    memcpy(data, aMsg->getBuffer(), aMsg->getLength());

    mCipher.encrypt(data, aMsg->getLength());
    mSocket->send(data, aMsg->getLength());

    SAFE_DELETE_ARRAY(data);
}

void
Client :: send(uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);

    uint8_t* data = new uint8_t[aLen];
    memcpy(data, aBuf, aLen);

    mCipher.encrypt(data, aLen);
    mSocket->send(data, aLen);

    SAFE_DELETE_ARRAY(data);
}

void
Client :: disconnect()
{
    // TODO
    mSocket->disconnect();
}
