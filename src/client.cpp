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
#include "msgloginproofa.h"
#include "msgloginchallenges.h"
#include "player.h"
#include "database.h"
#include "world.h"
#include <stdlib.h>

using namespace std;

Client :: Client(NetworkClient* aSocket, ICipher::Algorithm aAlgorithm)
    : mSocket(aSocket), mCipher(nullptr),
      mExchange(nullptr), mEncryptIV(nullptr), mDecryptIV(nullptr),
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
                static const uint8_t IV[Blowfish::BLOCK_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

                Blowfish* cipher = new Blowfish();
                cipher->generateKey((const uint8_t*)SEED, SEED_LEN);
                cipher->setIVs(IV, IV);

                mCipher = cipher;
                break;
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
    SAFE_DELETE_ARRAY(mEncryptIV);
    SAFE_DELETE_ARRAY(mDecryptIV);
}

void
Client :: save()
{
    printf("Calling save for %p ... %s\n",
           this, mPlayer != nullptr ? mPlayer->getName() : "");

    err_t err = ERROR_SUCCESS;
    int tries = 0;

    if (mPlayer != nullptr)
    {
        static const Database& db = Database::getInstance(); // singleton

        do
        {
            DOIF(err, db.savePlayer(*this));
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
Client :: generateExchangeRequest()
{
    ASSERT(mExchange == nullptr);

    // N.B. As the P & G keys are sent to the client in MsgLoginProofA, they can be random !
    // Although they are hard-coded in the client, they won't be used as-is.
    static const string P = "E7A69EBDF105F2A6BBDEAD7E798F76A209AD73FB466431E2E7352ED262F8C558F10BEFEA977DE9E21DCEE9B04D245F300ECCBBA03E72630556D011023F9E857F";
    static const string G = "05";

    mExchange = new DiffieHellman(P.c_str(), G.c_str());
    string pubKey = mExchange->generateRequest();

    mEncryptIV = new uint8_t[Blowfish::BLOCK_SIZE];
    mDecryptIV = new uint8_t[Blowfish::BLOCK_SIZE];
    for (int32_t i = 0; i < Blowfish::BLOCK_SIZE; ++i)
    {
        mEncryptIV[i] = (uint8_t)(rand() - rand());
        mDecryptIV[i] = (uint8_t)(rand() + rand());
    }

    MsgLoginProofA msg(mEncryptIV, mDecryptIV, P, G, pubKey);
    send(&msg);
}

void
Client :: handleExchangeResponse(uint8_t** aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);
    ASSERT(mExchange != nullptr);

    MsgLoginChallengeS msg(aBuf, aLen);
    msg.process(this, mEncryptIV, mDecryptIV);

    SAFE_DELETE(mExchange);
    SAFE_DELETE_ARRAY(mEncryptIV);
    SAFE_DELETE_ARRAY(mDecryptIV);
}

/* TQ seal. */
static const char SEAL[] = "TQServer";
static const size_t SEAL_LEN = strlen(SEAL);

void
Client :: send(const Msg* aMsg)
{
    ASSERT(aMsg != nullptr);

    const size_t len = aMsg->getLength() + SEAL_LEN;
    uint8_t* data = new uint8_t[len];

    memcpy(data, aMsg->getBuffer(), aMsg->getLength());
    memcpy(data + aMsg->getLength(), SEAL, SEAL_LEN);

    mCipher->encrypt(data, len);
    mSocket->send(data, len);

    SAFE_DELETE_ARRAY(data);
}

void
Client :: disconnect()
{
    mSocket->disconnect();
}
