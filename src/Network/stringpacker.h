/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_STRINGPACKER_H_
#define _COPS_V7_EMULATOR_STRINGPACKER_H_

#include "common.h"

/**
 * String packer for an internal buffer. All strings are prefixed with
 * their length in an unsigned 8-bit integer.
 *
 * When extracting string, the first string is at the index 0.
 */
class StringPacker
{
    // !!! class has pointer data members !!!
    PROHIBIT_COPY(StringPacker);

public:
    /**
     * Create a new packer around the specified buffer.
     *
     * @param aBuf[in]   a reference to the buffer
     */
    StringPacker(uint8_t* aBuf);

    /* destructor */
    ~StringPacker();

    /**
     * Append a string at the end of the buffer.
     *
     * @param aStr[in]     a c-string to append at the end of the buffer
     */
    void addString(const char* aStr);

    /**
     * Extract a string from the buffer.
     *
     * @param aOutBuf[out]  a reference to the external buffer that will receive the string
     * @param aLen[in]      the length of the buffer
     * @param aIndex[in]    the index of the string to retreive (0 is the first string)
     *
     * @retval TRUE on success
     * @returns FALSE otherwise
     */
    bool getString(char* aOutBuf, size_t aLen, uint8_t aIndex) const;

private:
    uint8_t* mBuf; //!< reference to the internal buffer
    uint8_t mStrCount; //!< the number of strings in the buffer
};

#endif // _COPS_V7_EMULATOR_STRINGPACKER_H_
