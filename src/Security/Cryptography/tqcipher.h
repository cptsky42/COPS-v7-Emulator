/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_TQCIPHER_H
#define _COPS_V7_EMULATOR_TQCIPHER_H

#include "common.h"

/**
 * TQ Digital's cipher used by the AccServer & the MsgServer of the
 * game Era of Faith. It uses a 4096-bit key, based from two 32-bit integer,
 * with two 16-bit incremental counter. The cipher is barely a XOR cipher.
 *
 * The following implementation has a memory footprint of 1 KiO.
 */
class TqCipher
{
public:
    /** The initialization vector size in bytes. It is used as the primary key. */
    static const size_t IV_SIZE = 512;
    /** The key size in bytes. It is used as the alternative key. */
    static const size_t KEY_SIZE = 512;

public:
    /**
     * Create a new instance of the cipher where the IV and the key is
     * zero-filled.
     */
    TqCipher();

    /* destructor */
    ~TqCipher();

public:
    /**
     * Generate the initialization vector based on the P & G integers which
     * are respectively two 32-bit integers.
     *
     * @param[in] aP  the P value of the cipher
     * @param[in] aG  the G value of the cipher
     */
    void generateIV(int32_t aP, int32_t aG);

    /**
     * Generate the key based on the A & B integers which are respectively two
     * 32-bit integers. The encryption counter will be resetted and the key will
     * be used instead of the IV for decrypting the data.
     *
     * @param[in] aA  the A value of the cipher
     * @param[in] aB  the B value of the cipher
     */
    void generateKey(int32_t aA, int32_t aB);

    /**
     * Encrypt n octet(s) with the cipher.
     *
     *
     * @param[in,out] aBuf          the buffer that will be encrypted
     * @param[in]     aLen          the number of octets to encrypt
     */
    void encrypt(uint8_t* aBuf, size_t aLen);

    /**
     * Decrypt n octet(s) with the cipher.
     *
     *
     * @param[in,out] aBuf          the buffer that will be decrypted
     * @param[in]     aLen          the number of octets to decrypt
     */
    void decrypt(uint8_t* aBuf, size_t aLen);

    /**
     * Reset both counters.
     */
    void resetCounters();

private:
    uint16_t mEnCounter; //!< Internal encryption counter.
    uint16_t mDeCounter; //!< Internal decryption counter.

    uint8_t mIV[IV_SIZE]; //!< Initialization vector
    uint8_t mKey[KEY_SIZE]; //!< Alternative key

    bool mUseKey; //!< Determine if the cipher must use the alt. key
};

#endif // _COPS_V7_EMULATOR_TQCIPHER_H
