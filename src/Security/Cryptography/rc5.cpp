/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "rc5.h"
#include <string.h> // memset
#include <algorithm> // max()

using namespace std;

#ifndef _MSC_VER // Visual Studio won't throw an error...
const size_t RC5::RC5_SUB;
const size_t RC5::RC5_KEY;
#endif

RC5 :: RC5()
{
    // security purpose only...
    memset(mKey, 0, sizeof(mKey));
    memset(mSub, 0, sizeof(mSub));
}

RC5 :: ~RC5()
{

}

void
RC5 :: generateKey(uint8_t aSeed[KEY_SIZE])
{
    ASSERT(aSeed != nullptr);

    uint32_t* seed = (uint32_t*)aSeed;
    for (size_t i = 0; i < RC5::RC5_KEY; ++i)
    {
        mKey[i] = seed[i];
        #if BYTE_ORDER == BIG_ENDIAN
        mKey[i] = bswap32(mKey[i]);
        #endif
    }

    mSub[0] = RC5::RC5_PW32;
    size_t i, j;
    for (i = 1; i < RC5::RC5_SUB; ++i)
    {
        mSub[i] = mSub[i - 1] - RC5::RC5_QW32;
    }

    uint32_t x, y;
    i = j = 0;
    x = y = 0;
    for (size_t k = 0, size = 3 * max(RC5::RC5_KEY, RC5::RC5_SUB); k < size; ++k)
    {
        mSub[i] = rotl((mSub[i] + x + y), 3);
        x = mSub[i];
        i = (i + 1) % RC5::RC5_SUB;
        mKey[j] = rotl((mKey[j] + x + y), (x + y));
        y = mKey[j];
        j = (j + 1) % RC5::RC5_KEY;
    }
}

void
RC5 :: encrypt(uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);
    ASSERT(aLen > 0 && aLen % RC5::BLOCK_SIZE == 0);

    uint32_t* buf = (uint32_t*)aBuf;
    for (size_t i = 0, len = aLen / RC5::BLOCK_SIZE; i < len; ++i)
    {
        uint32_t a = buf[2 * i];
        uint32_t b = buf[2 * i + 1];

        #if BYTE_ORDER == BIG_ENDIAN
        a = bswap32(a);
        b = bswap32(b);
        #endif

        uint32_t le = a + mSub[0];
        uint32_t re = b + mSub[1];
        for (size_t j = 1; j < RC5::ROUNDS; ++j)
        {
            le = rotl((le ^ re), re) + mSub[2 * j];
            re = rotl((re ^ le), le) + mSub[2 * j + 1];
        }

        #if BYTE_ORDER == BIG_ENDIAN
        le = bswap32(le);
        re = bswap32(re);
        #endif

        buf[2 * i] = le;
        buf[2 * i + 1] = re;
    }
}

void
RC5 :: decrypt(uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);
    ASSERT(aLen > 0 && aLen % RC5::BLOCK_SIZE == 0);

    uint32_t* buf = (uint32_t*)aBuf;
    for (size_t i = 0, len = aLen / RC5::BLOCK_SIZE; i < len; ++i)
    {
        uint32_t ld = buf[2 * i];
        uint32_t rd = buf[2 * i + 1];

        #if BYTE_ORDER == BIG_ENDIAN
        ld = bswap32(ld);
        rd = bswap32(rd);
        #endif

        for (size_t j = RC5::ROUNDS; j >= 1; --j)
        {
            rd = rotr((rd - mSub[2 * j + 1]), ld) ^ ld;
            ld = rotr((ld - mSub[2 * j]), rd) ^ rd;
        }
        uint32_t a = rd - mSub[1];
        uint32_t b = ld - mSub[0];

        #if BYTE_ORDER == BIG_ENDIAN
        a = bswap32(a);
        b = bswap32(b);
        #endif

        buf[2 * i] = b;
        buf[2 * i + 1] = a;
    }
}
