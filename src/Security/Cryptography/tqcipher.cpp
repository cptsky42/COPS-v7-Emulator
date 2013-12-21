/**
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
    memset(mKey, 0, sizeof(mKey));

    mUseKey = false;
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

    static const size_t middle = TqCipher::IV_SIZE  / 2;
    for (size_t i = 0, len = middle; i < len; ++i)
    {
        mIV[i] = p[0];
        mIV[i + middle] = g[0];
        p[0] = (uint8_t)(((uint8_t)(p[0] << p[2]) + p[1]) * p[0] + p[3]);
        g[0] = (uint8_t)((g[0] * g[2] - g[1]) * g[0] - g[3]);
    }
}

void
TqCipher :: generateKey(int32_t aA, int32_t aB)
{
    uint32_t val1 = (uint32_t)(((aA + aB) ^ 0x4321) ^ aA);
    uint32_t val2 = val1 * val1;

    #if BYTE_ORDER == BIG_ENDIAN
    val1 = bswap32(val1);
    val2 = bswap32(val2);
    #endif

    uint8_t* ptr1 = (uint8_t*)&val1;
    uint8_t* ptr2 = (uint8_t*)&val2;

    static const size_t middle = TqCipher::IV_SIZE  / 2;
    for (size_t i = 0, len = middle; i < len; ++i)
    {
        mKey[i] = (uint8_t)(mIV[i] ^ ptr1[i % 4]);
        mKey[i + middle] = (uint8_t)(mIV[i + middle] ^ ptr2[i % 4]);
    }

    mEnCounter = 0;
    mUseKey = true;
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

    uint8_t* key1 = mUseKey ? mKey : mIV;
    uint8_t* key2 = key1 + (TqCipher::IV_SIZE  / 2);

    for (size_t i = 0; i < aLen; ++i)
    {
        aBuf[i] ^= key1[(uint8_t)mDeCounter];
        aBuf[i] ^= key2[(uint8_t)(mDeCounter >> 8)];
        ++mDeCounter;
    }
}

void
TqCipher :: resetCounters()
{
    mEnCounter = 0;
    mDeCounter = 0;
}
