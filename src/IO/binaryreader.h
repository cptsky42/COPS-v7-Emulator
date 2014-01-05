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

#ifndef _COPS_V7_EMULATOR_BINARY_READER_H_
#define _COPS_V7_EMULATOR_BINARY_READER_H_

#include "common.h"
#include <string>

/**
  * Initializes a new instance of the BinaryReader class based on the
  * specified stream.
  * On a Big-Endian architecture, all integers will be swapped.
  */
class BinaryReader
{
public:
    /**
      * Create a new BinaryReader and open the underlying stream
      * based on the path and the mode.
      *
      * @param[in]    aPath   the path to the target file
      */
    BinaryReader(const char* aPath);

    /**
      * Create a new BinaryReader and open the underlying stream
      * based on the path and the mode.
      *
      * @param[in]    aPath   the path to the target file
      */
    BinaryReader(const std::string& aPath);

    /* destructor */
    ~BinaryReader();

    /**
      * Gets the current position of the stream used by the BinaryReader.
      *
      * @returns the current position of the stream
      */
    long tell();

    /**
      * Changes the current position of the stream based on the seeking
      * mode.
      *
      * @param[in]    aOffset     the relative offset to use
      * @param[in]    aWhence     the seeking mode
      *                           SEEK_SET based on the start of the file
      *                           SEEK_CUR based on the current position
      *                           SEEK_END based on the end of the file
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_SEEK on seeking error
      * @returns BL error code otherwise
      */
    err_t seek(long aOffset, int aWhence);

    /**
      * Closes the current reader and the underlying stream.
      * All internal buffers associated with the stream are disassociated
      * from it and flushed
      */
    void close();

    /**
      * Reads the specified number of bytes from the stream.
      *
      * @param[in, out] aBuf    a pointer to the buffer to read data into
      * @param[in]      aLen  the number of bytes to read
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_READ on reading error
      * @returns BL error code otherwise
      */
    err_t read(void* aBuf, size_t aCount);

    /**
      * Reads a 8-bit unsigned integer value from the current stream and
      * advances the current position of the stream by one byte.
      *
      * @param[out]    aOutVal    a reference to the integer that will
      *                           receive the value
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_READ on reading error
      * @returns BL error code otherwise
      */
    err_t readUInt8(uint8_t& aOutVal);

    /**
      * Reads a 16-bit unsigned integer value from the current stream and
      * advances the current position of the stream by two bytes.
      *
      * @param[out]    aOutVal    a reference to the integer that will
      *                           receive the value
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_READ on reading error
      * @returns BL error code otherwise
      */
    err_t readUInt16(uint16_t& aOutVal);

    /**
      * Reads a 32-bit unsigned integer value from the current stream and
      * advances the current position of the stream by four bytes.
      *
      * @param[out]    aOutVal    a reference to the integer that will
      *                           receive the value
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_READ on reading error
      * @returns BL error code otherwise
      */
    err_t readUInt32(uint32_t& aOutVal);

    /**
      * Reads a 64-bit unsigned integer value from the current stream and
      * advances the current position of the stream by heigt bytes.
      *
      * @param[out]    aOutVal    a reference to the integer that will
      *                           receive the value
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_READ on reading error
      * @returns BL error code otherwise
      */
    err_t readUInt64(uint64_t& aOutVal);

    /**
      * Reads a 8-bit signed integer value from the current stream and
      * advances the current position of the stream by one byte.
      *
      * @param[out]    aOutVal    a reference to the integer that will
      *                           receive the value
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_READ on reading error
      * @returns BL error code otherwise
      */
    err_t readInt8(int8_t& aOutVal);

    /**
      * Reads a 16-bit signed integer value from the current stream and
      * advances the current position of the stream by two bytes.
      *
      * @param[out]    aOutVal    a reference to the integer that will
      *                           receive the value
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_READ on reading error
      * @returns BL error code otherwise
      */
    err_t readInt16(int16_t& aOutVal);

    /**
      * Reads a 32-bit signed integer value from the current stream and
      * advances the current position of the stream by four bytes.
      *
      * @param[out]    aOutVal    a reference to the integer that will
      *                           receive the value
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_READ on reading error
      * @returns BL error code otherwise
      */
    err_t readInt32(int32_t& aOutVal);

    /**
      * Reads a 64-bit signed integer value from the current stream and
      * advances the current position of the stream by heigt bytes.
      *
      * @param[out]    aOutVal    a reference to the integer that will
      *                           receive the value
      *
      * @retval BL_ERROR_SUCCESS on success
      * @retval BL_ERROR_FILE_READ on reading error
      * @returns BL error code otherwise
      */
    err_t readInt64(int64_t& aOutVal);

private:
    template<class T>
    err_t read(T& aOutVal);

private:
    FILE* mStream; //!< the base stream of the reader
};

#endif /* _COPS_V7_EMULATOR_BINARY_READER_H_ */
