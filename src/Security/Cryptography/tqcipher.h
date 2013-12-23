/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_TQ_CIPHER_H_
#define _COPS_V7_EMULATOR_TQ_CIPHER_H_

#include "common.h"
#include "icipher.h"

/**
 * TQ Digital's cipher used by the AccServer of the game Conquer Online.
 * It uses a 4096-bit key, based from two 32-bit integer, with two 16-bit
 * incremental counter. The cipher is barely a XOR cipher.
 *
 * The following implementation has a memory footprint of 1 KiO.
 */
class TqCipher : public ICipher
{
public:
    /** The initialization vector size in bytes. It is used as the primary key. */
    static const size_t IV_SIZE = 512;

public:
    /**
     * Create a new instance of the cipher where the IV and the key is
     * zero-filled.
     */
    TqCipher();

    /* destructor */
    virtual ~TqCipher();

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
     * Encrypt n octet(s) with the cipher.
     *
     *
     * @param[in,out] aBuf          the buffer that will be encrypted
     * @param[in]     aLen          the number of octets to encrypt
     */
    virtual void encrypt(uint8_t* aBuf, size_t aLen);

    /**
     * Decrypt n octet(s) with the cipher.
     *
     *
     * @param[in,out] aBuf          the buffer that will be decrypted
     * @param[in]     aLen          the number of octets to decrypt
     */
    virtual void decrypt(uint8_t* aBuf, size_t aLen);

public:
    /** Get the algorithm used by the cipher. */
    virtual ICipher::Algorithm getAlgorithm() const { return ICipher::TQ_CIPHER; }

private:
    uint16_t mEnCounter; //!< Internal encryption counter.
    uint16_t mDeCounter; //!< Internal decryption counter.

    uint8_t mIV[IV_SIZE]; //!< Initialization vector
};

#endif // _COPS_V7_EMULATOR_TQ_CIPHER_H_
