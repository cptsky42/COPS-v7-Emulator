/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgloginchallenges.h"
#include "client.h"
#include "diffiehellman.h"
#include "blowfish.h"
#include <string>

using namespace std;

MsgLoginChallengeS :: MsgLoginChallengeS(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgLoginChallengeS :: ~MsgLoginChallengeS()
{

}

void
MsgLoginChallengeS :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);

    Client& client = *aClient;

    uint8_t* ptr = mBuf;
    MsgInfo info; // HACK for member size...

    mBuf += sizeof(info.Padding); // Padding
    mBuf += sizeof(int32_t); // Size
    mBuf += *((int32_t*)(ptr)) + sizeof(int32_t); // JunkSize + Junk

    int32_t b_len = *((int32_t*)(ptr));
    mBuf += sizeof(int32_t);

    char* b_key = new char[b_len];
    for (int32_t i = 0; i < b_len; ++i)
    {
        b_key[i] = (char)(mBuf[i]);
    }

    DiffieHellman& exchange = client.getExchange();
    string s_key = exchange.handleResponse(b_key);
    SAFE_DELETE_ARRAY(b_key);

    ASSERT(client.getCipher().getAlgorithm() == ICipher::BLOWFISH);
    Blowfish* cipher = (Blowfish*)&client.getCipher();
    cipher->generateKey((const uint8_t*)s_key.c_str(), s_key.length());
    //TODO: cipher->setIVs(); with the previously given IVs

    client.setStatus(Client::NORMAL);
}

void
MsgLoginChallengeS :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo info; // HACK for member size
    uint8_t* ptr = aBuf + sizeof(info.Padding);

    *((int32_t*)(ptr)) = bswap32(*((int32_t*)(ptr))); // Size
    ptr += sizeof(uint32_t);

    *((int32_t*)(ptr)) = bswap32(*((int32_t*)(ptr))); // Junk Size
    ptr += *((int32_t*)(ptr));
    ptr += sizeof(int32_t);

    *((int32_t*)(ptr)) = bswap32(*((int32_t*)(ptr))); // S Size
}
