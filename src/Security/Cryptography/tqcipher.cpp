/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "tqcipher.h"
#include <string.h> // memset

TqCipher :: TqCipher()
{
    mEnCounter = 0;
    mDeCounter = 0;

    // security purpose only...
    memset(mIV, 0, sizeof(mIV));
}

TqCipher :: ~TqCipher()
{

}

void
TqCipher :: generateIV(int32_t aP, int32_t aG)
{
    #if BYTE_ORDER == BIG_ENDIAN
    aP = bswap32(aP);
    aG = bswap32(aG);
    #endif

    uint8_t* p = (uint8_t*)&aP;
    uint8_t* g = (uint8_t*)&aG;

    uint8_t* key1 = mIV;
    uint8_t* key2 = key1 + (TqCipher::IV_SIZE  / 2);

    for (size_t i = 0, len = (TqCipher::IV_SIZE  / 2); i < len; ++i)
    {
        key1[i] = p[0];
        key2[i] = g[0];
        p[0] = (uint8_t)((p[1] + (uint8_t)(p[0] * p[2])) * p[0] + p[3]);
        g[0] = (uint8_t)((g[1] - (uint8_t)(g[0] * g[2])) * g[0] + g[3]);
    }
}

void
TqCipher :: encrypt(uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);
    ASSERT(aLen > 0);

    uint8_t* key1 = mIV;
    uint8_t* key2 = key1 + (TqCipher::IV_SIZE  / 2);

    for (size_t i = 0; i < aLen; ++i)
    {
        aBuf[i] ^= UINT8_C(0xAB);
        aBuf[i] = (uint8_t)(aBuf[i] << 4 | aBuf[i] >> 4);
        aBuf[i] ^= key1[(uint8_t)mEnCounter];
        aBuf[i] ^= key2[(uint8_t)(mEnCounter >> 8)];
        ++mEnCounter;
    }
}

void
TqCipher :: decrypt(uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);
    ASSERT(aLen > 0);

    uint8_t* key1 = mIV;
    uint8_t* key2 = key1 + (TqCipher::IV_SIZE  / 2);

    for (size_t i = 0; i < aLen; ++i)
    {
        aBuf[i] ^= UINT8_C(0xAB);
        aBuf[i] = (uint8_t)(aBuf[i] << 4 | aBuf[i] >> 4);
        aBuf[i] ^= key1[(uint8_t)mDeCounter];
        aBuf[i] ^= key2[(uint8_t)(mDeCounter >> 8)];
        ++mDeCounter;
    }
}
