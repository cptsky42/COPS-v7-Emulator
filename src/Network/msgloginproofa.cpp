/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgloginproofa.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

MsgLoginProofA :: MsgLoginProofA(const uint8_t* aEncryptIV, const uint8_t* aDecryptIV,
                                 const string& aP, const string& aG, const string& aA)
    : Msg(sizeof(MsgInfo) + aP.length() + aG.length() + aA.length() - 3)
{
    MsgInfo* info = (MsgInfo*)mBuf;
    srand(time(NULL));

    for (size_t i = 0; i < MsgLoginProofA::PADDING_LEN; ++i)
    {
        info->Padding[i] = rand();
    }

    info->JunkSize = MsgLoginProofA::JUNK_LEN;
    for (size_t i = 0; i < MsgLoginProofA::JUNK_LEN; ++i)
    {
        info->Junk[i] = rand();
    }

    // Will include the TQServer seal (added by send())...
    info->Size = (int32_t)(mLen - MsgLoginProofA::PADDING_LEN + 8);

    info->EnIVSize = Blowfish::BLOCK_SIZE;
    memcpy(info->EnIV, aEncryptIV, Blowfish::BLOCK_SIZE);
    info->DeIVSize = Blowfish::BLOCK_SIZE;
    memcpy(info->DeIV, aDecryptIV, Blowfish::BLOCK_SIZE);

    uint8_t* ptr = (uint8_t*)&info->PSize;

    *((int32_t*)(ptr)) = (int32_t)aP.length();
    ptr += sizeof(int32_t);
    memcpy(ptr, aP.c_str(), aP.length());
    ptr += aP.length();

    *((int32_t*)(ptr)) = (int32_t)aG.length();
    ptr += sizeof(int32_t);
    memcpy(ptr, aG.c_str(), aG.length());
    ptr += aG.length();

    *((int32_t*)(ptr)) = (int32_t)aA.length();
    ptr += sizeof(int32_t);
    memcpy(ptr, aA.c_str(), aA.length());
    ptr += aA.length();
}

MsgLoginProofA :: ~MsgLoginProofA()
{

}

void
MsgLoginProofA :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;
    uint8_t* ptr = (uint8_t*)&info->PSize;
    int32_t size = 0;

    info->Size = bswap32(info->Size);
    info->JunkSize = bswap32(info->JunkSize);
    info->EnIVSize = bswap32(info->EnIVSize);
    info->DeIVSize = bswap32(info->DeIVSize);

    size = *((int32_t*)(ptr));
    *((int32_t*)(ptr)) = bswap32(size);
    ptr += sizeof(int32_t) + size;

    size = *((int32_t*)(ptr));
    *((int32_t*)(ptr)) = bswap32(size);
    ptr += sizeof(int32_t) + size;

    size = *((int32_t*)(ptr));
    *((int32_t*)(ptr)) = bswap32(size);
    ptr += sizeof(int32_t) + size;
}

