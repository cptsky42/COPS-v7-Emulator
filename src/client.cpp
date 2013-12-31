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
                static const uint8_t IV[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

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
Client :: generateExchangeRequest()
{
    ASSERT(mExchange == nullptr);

    static const string P = "E7A69EBDF105F2A6BBDEAD7E798F76A209AD73FB466431E2E7352ED262F8C558F10BEFEA977DE9E21DCEE9B04D245F300ECCBBA03E72630556D011023F9E857F";
    static const string G = "05";

    Blowfish* cipher = (Blowfish*)mCipher;
    ASSERT(mCipher->getAlgorithm() == ICipher::BLOWFISH);

    mExchange = new DiffieHellman(P.c_str(), G.c_str());
    string pubKey = mExchange->generateRequest();

    mEncryptIV = new uint8_t[Blowfish::BLOCK_SIZE];
    memcpy(mEncryptIV, cipher->getEncryptIV(), Blowfish::BLOCK_SIZE); // TODO random
    mDecryptIV = new uint8_t[Blowfish::BLOCK_SIZE];
    memcpy(mDecryptIV, cipher->getDecryptIV(), Blowfish::BLOCK_SIZE); // TODO random

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

    mCipher->encrypt(data, aMsg->getLength() + SEAL_LEN);
    mSocket->send(data, aMsg->getLength() + SEAL_LEN);

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

    mCipher->encrypt(data, aLen + SEAL_LEN);
    mSocket->send(data, aLen + SEAL_LEN);

    SAFE_DELETE_ARRAY(data);
}

void
Client :: disconnect()
{
    // TODO
    mSocket->disconnect();
}
