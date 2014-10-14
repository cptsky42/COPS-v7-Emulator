/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_ICIPHER_H_
#define _COPS_V7_EMULATOR_ICIPHER_H_

#include "common.h"

/**
 * Generic interface for all ciphers.
 */
class ICipher
{
public:
    /** All supported algorithms. */
    enum Algorithm
    {
        /** TQ's custom algorithm. */
        TQ_CIPHER,
        /** Blowfish (CFB64) algorithm. */
        BLOWFISH,
        /** CAST-5 (CFB64) algorithm. */
        CAST5,
        /** RC5 algorithm. */
        RC5
    };

public:
    /* constructor */
    ICipher() { }

    /* destrctor */
    virtual ~ICipher() { }

public:
    /**
     * Encrypt n octet(s) with the cipher.
     *
     *
     * @param[in,out] aBuf          the buffer that will be encrypted
     * @param[in]     aLen          the number of octets to encrypt
     */
    virtual void encrypt(uint8_t* aBuf, size_t aLen) = 0;

    /**
     * Decrypt n octet(s) with the cipher.
     *
     *
     * @param[in,out] aBuf          the buffer that will be decrypted
     * @param[in]     aLen          the number of octets to decrypt
     */
    virtual void decrypt(uint8_t* aBuf, size_t aLen) = 0;

public:
    /** Get the algorithm used by the cipher. */
    virtual Algorithm getAlgorithm() const = 0;
};

#endif // _COPS_V7_EMULATOR_ICIPHER_H_
