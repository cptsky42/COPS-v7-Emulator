/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Taken from :
 * ****** BARLab - Open Source ******
 * Copyright (C) 2012 BARLab
 * Copyright (C) 2012 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "binaryreader.h"
#include "finder.h"
#include <stdio.h>
#include <string.h> // strerror

using namespace std;

BinaryReader :: BinaryReader(const char* aPath)
{
    mStream = nullptr;
    ASSERT(aPath != nullptr && aPath[0] != '\0');

    mStream = fopen(aPath, "rb");
    if (mStream == nullptr)
    {
        LOG(ERROR, "failed to open '%s' in '%s' : %s", aPath, "rb", strerror(errno));
        ASSERT(false);
    }
}

BinaryReader :: BinaryReader(const string& aPath)
{
    mStream = nullptr;
    ASSERT(&aPath != nullptr);
    ASSERT(!aPath.empty());

    mStream = fopen(aPath.c_str(), "rb");
    if (mStream == nullptr)
    {
        LOG(ERROR, "failed to open '%s' in '%s' : %s", aPath.c_str(), "rb", strerror(errno));
        ASSERT(false);
    }
}

BinaryReader :: ~BinaryReader()
{
    close();
}

long
BinaryReader :: tell()
{
    return ftell(mStream);
}

err_t
BinaryReader :: seek(long aOffset, int aWhence)
{
    return fseek(mStream, aOffset, aWhence) == 0 ? ERROR_SUCCESS : ERROR_SEEK;
}

void
BinaryReader :: close()
{
    if (mStream != nullptr)
    {
        fclose(mStream);
        mStream = nullptr;
    }
}

err_t
BinaryReader :: readUInt8(uint8_t& aOutVal)
{
    int8_t* ptr = (int8_t*)(&aOutVal);
    return readInt8(*ptr);
}

err_t
BinaryReader :: readUInt16(uint16_t& aOutVal)
{
    int16_t* ptr = (int16_t*)(&aOutVal);
    return readInt16(*ptr);
}

err_t
BinaryReader :: readUInt32(uint32_t& aOutVal)
{
    int32_t* ptr = (int32_t*)(&aOutVal);
    return readInt32(*ptr);
}

err_t
BinaryReader :: readUInt64(uint64_t& aOutVal)
{
    int64_t* ptr = (int64_t*)(&aOutVal);
    return readInt64(*ptr);
}

template<class T>
err_t
BinaryReader :: read(T& aOutVal)
{
    err_t err = ERROR_SUCCESS;

    if (fread(&aOutVal, sizeof(aOutVal), 1, mStream) != 1)
    {
        err = ERROR_READ_FAULT;
    }

    return err;
}

template err_t BinaryReader :: read<int8_t>(int8_t& aOutVal);
template err_t BinaryReader :: read<int16_t>(int16_t& aOutVal);
template err_t BinaryReader :: read<int32_t>(int32_t& aOutVal);
template err_t BinaryReader :: read<int64_t>(int64_t& aOutVal);

err_t
BinaryReader :: read(void* aBuf, size_t aLen)
{
    ASSERT_ERR(aBuf != nullptr, ERROR_INVALID_PARAMETER);
    ASSERT_ERR(aLen > 0, ERROR_INVALID_PARAMETER);
    ASSERT_ERR(mStream != nullptr, ERROR_INVALID_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_INVALID_STATE);

    err_t err = ERROR_SUCCESS;

    if (fread(aBuf, sizeof(uint8_t), aLen, mStream) != aLen)
    {
        err = ERROR_READ_FAULT;
    }

    return err;
}

err_t
BinaryReader :: readInt8(int8_t& aOutVal)
{
    ASSERT_ERR(&aOutVal != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(mStream != nullptr, ERROR_INVALID_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_INVALID_STATE);

    err_t err = ERROR_SUCCESS;

    DOIF(err, read<int8_t>(aOutVal));

    return err;
}

err_t
BinaryReader :: readInt16(int16_t& aOutVal)
{
    ASSERT_ERR(&aOutVal != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(mStream != nullptr, ERROR_INVALID_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_INVALID_STATE);

    err_t err = ERROR_SUCCESS;

    DOIF(err, read<int16_t>(aOutVal));
    #if BYTE_ORDER == BIG_ENDIAN
    aOutVal = bswap16(aOutVal);
    #endif

    return err;
}

err_t
BinaryReader :: readInt32(int32_t& aOutVal)
{
    ASSERT_ERR(&aOutVal != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(mStream != nullptr, ERROR_INVALID_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_INVALID_STATE);

    err_t err = ERROR_SUCCESS;

    DOIF(err, read<int32_t>(aOutVal));
    #if BYTE_ORDER == BIG_ENDIAN
    aOutVal = bswap32(aOutVal);
    #endif

    return err;
}

err_t
BinaryReader :: readInt64(int64_t& aOutVal)
{
    ASSERT_ERR(&aOutVal != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(mStream != nullptr, ERROR_INVALID_STATE);
    ASSERT_ERR(feof(mStream) == 0, ERROR_INVALID_STATE);

    err_t err = ERROR_SUCCESS;

    DOIF(err, read<int64_t>(aOutVal));
    #if BYTE_ORDER == BIG_ENDIAN
    aOutVal = bswap64(aOutVal);
    #endif

    return err;
}
