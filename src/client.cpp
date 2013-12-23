/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "client.h"
#include "networkclient.h"
#include "tqcipher.h"
#include "blowfish.h"
#include "diffiehellman.h"
#include "msg.h"
#include "player.h"
#include "database.h"
#include "world.h"
#include <stdlib.h>

Client :: Client(NetworkClient* aSocket, ICipher::Algorithm aAlgorithm)
    : mSocket(aSocket), mCipher(nullptr), mExchange(nullptr),
      mAccountID(-1), mAccLvl(0), mFlags(0),
      mPlayer(nullptr)
{
    ASSERT(aSocket != nullptr);

    switch (aAlgorithm)
    {
        case ICipher::TQ_CIPHER:
            {
                TqCipher* cipher = new TqCipher();
                cipher->generateIV(0x13FA0F9D, 0x6D5C7962);

                mCipher = cipher;
                break;
            }
        case ICipher::BLOWFISH:
            {
                static const char SEED[] = "DR654dt34trg4UI6";
                static const size_t SEED_LEN = strlen(SEED);

                Blowfish* cipher = new Blowfish();
                cipher->generateKey((const uint8_t*)SEED, SEED_LEN);

                mCipher = cipher;
            }
        default:
            ASSERT(false); // unknown cipher
            break;
    }

    mStatus = Client::NOT_AUTHENTICATED;
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

    SAFE_DELETE(mCipher);
    SAFE_DELETE(mExchange);
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
            LOG(ERROR, "Failed to save player %s for %p.",
                mPlayer->getName(), this);
        }
    }
}

void
Client :: send(Msg* aMsg)
{
    ASSERT(aMsg != nullptr);

    // TODO move me
    static const char SEAL[] = "TQServer";
    static const size_t SEAL_LEN = strlen(SEAL);

    uint8_t* data = new uint8_t[aMsg->getLength() + SEAL_LEN];
    memcpy(data, aMsg->getBuffer(), aMsg->getLength());
    memcpy(data + aMsg->getLength(), SEAL, SEAL_LEN);

    mCipher->encrypt(data, aMsg->getLength());
    mSocket->send(data, aMsg->getLength());

    SAFE_DELETE_ARRAY(data);
}

void
Client :: send(uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);

    // TODO move me
    static const char SEAL[] = "TQServer";
    static const size_t SEAL_LEN = strlen(SEAL);

    uint8_t* data = new uint8_t[aLen + SEAL_LEN];
    memcpy(data, aBuf, aLen);
    memcpy(data + aLen, SEAL, SEAL_LEN);

    mCipher->encrypt(data, aLen);
    mSocket->send(data, aLen);

    SAFE_DELETE_ARRAY(data);
}

void
Client :: disconnect()
{
    // TODO
    mSocket->disconnect();
}
