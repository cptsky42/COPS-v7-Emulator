/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "stringpacker.h"
#include <string.h>

StringPacker :: StringPacker(uint8_t* aBuf)
{
    ASSERT(aBuf != nullptr);

    mBuf = aBuf;
    mStrCount = *aBuf;
}

StringPacker :: ~StringPacker()
{

}

void
StringPacker :: addString(const char* aStr)
{
    ASSERT(strlen(aStr) <= UINT8_MAX);

    char* ptr = (char*)(mBuf + 1);
    for (uint8_t i = 0; i < mStrCount; ++i)
    {
        uint8_t len = (uint8_t)*(ptr++);
        ptr += len;
    }

    *((uint8_t*)ptr++) = (uint8_t)strlen(aStr);
    strcpy(ptr, aStr);

    *mBuf = ++mStrCount;
}

bool
StringPacker :: getString(char* aOutBuf, size_t aLen, uint8_t aIndex) const
{
    ASSERT(aOutBuf != nullptr);
    ASSERT(aLen > 0);

    bool success = false;

    if (aIndex < mStrCount)
    {
        char* ptr = (char*)(mBuf + 1);
        for (uint8_t i = 0; i < aIndex; ++i)
        {
            uint8_t len = (uint8_t)*(ptr++);
            ptr += len;
        }

        uint8_t len = *((uint8_t*)ptr++);
        if (aLen > len)
        {
            memcpy(aOutBuf, ptr, len);
            aOutBuf[len] = '\0';

            success = true;
        }
    }

    return success;
}
