/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2014 Jean-Philippe Boivin
 * Copyright (C) 1996 Carlisle Adams and Stafford Tavares
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_CAST5_CFB64_H_
#define _COPS_V7_EMULATOR_CAST5_CFB64_H_

#include "common.h"
#include "icipher.h"

/**
 * CAST-128 (alternatively CAST5) is a symmetric-key block cipher.
 * The algorithm was created in 1996 by Carlisle Adams and Stafford Tavares.
 *
 * CAST-128 is a 12- or 16-round Feistel network with a 64-bit block size
 * and a key size of between 40 to 128 bits (but only in 8-bit increments).
 * The full 16 rounds are used when the key size is longer than 80 bits.
 *
 * This implementation uses the Cipher Feedback 64 mode on stream.
 * This implementation requires less than 1 KiO of memory.
 */
class CAST5 : public ICipher
{
public:
    /** The block size of the algorithm in bits (must be 64) */
    static const int32_t BLOCK_SIZE = INT32_C(8);
    /** The number of round of the algorithm (must be 16) */
    static const int32_t ROUNDS = INT32_C(16);
    /** The key size of the algorithm in bytes (must be 16) */
    static const int32_t KEY_SIZE = INT32_C(16);

public:
    /* constructor */
    CAST5();

    /* destructor */
    virtual ~CAST5();

    /**
     * Generates the key based on the specified seed.
     * Positions of the IVs will be set to zero.
     *
     * @param[in]     aSeed         the seed that will be used
     * @param[in]     aLen          the number of octets of the seed
     */
    void generateKey(const uint8_t* aSeed, size_t aLen);

    /**
     * Encrypts n octet(s) with the CAST-5 algorithm and the CFB64 mode.
     *
     *
     * @param[in,out] aBuf          the buffer that will be encrypted
     * @param[in]     aLen          the number of octets to encrypt
     */
    virtual void encrypt(uint8_t* aBuf, size_t aLen);

    /**
     * Encrypts n octet(s) with the CAST-5 algorithm and the CFB64 mode.
     *
     *
     * @param[in,out] aBuf          the buffer that will be encrypted
     * @param[in]     aLen          the number of octets to encrypt
     */
    virtual void decrypt(uint8_t* aBuf, size_t aLen);

    /**
     * Sets the encryption IV and/or the decryption IV with the given value.
     * This function is specific to the CFB64 mode.
     *
     * @param[in]     aEnvIV        the new encryption IV
     * @param[in]     aDecIV        the new decryption IV
     */
    void setIVs(const uint8_t aEncIV[CAST5::BLOCK_SIZE],
                const uint8_t aDecIV[CAST5::BLOCK_SIZE]);

public:
    /** Get the algorithm used by the cipher. */
    virtual ICipher::Algorithm getAlgorithm() const { return ICipher::CAST5; }

    /** Get the encryption IV. */
    const uint8_t* getEncryptIV() { return mEnIV; }
    /** Get the decryption IV. */
    const uint8_t* getDecryptIV() { return mDeIV; }

private:
    /**
     * Applies the specified three operations on the t, lv, and rv values following
     * CAST-5 algorithm.
     */
    void E_CAST(uint32_t n, uint32_t& t, uint32_t& lv, uint32_t& rv, uint8_t op1, uint8_t op2, uint8_t op3);

    /**
     * Encrypts a block with the CAST-5 algorithm.
     *
     * @param[in,out] aBlock        the block to be encrypted
     */
    void encipher(uint8_t aBlock[CAST5::BLOCK_SIZE]);

    /**
     * Decrypts a block with the CAST-5 algorithm.
     *
     * @param[in,out] aBlock        the block to be decrypted
     */
    void decipher(uint8_t aBlock[CAST5::BLOCK_SIZE]);


private:
    /* Unit tests for CAST-5. */
    friend bool test_cast5();

private:
    static const uint32_t S[8][256]; //!< CAST-5 S-Boxes

    uint32_t mKey[CAST5::KEY_SIZE * 2]; //!< Generated key
    bool mShortKey; //!< Whether the seed was short or not

    //** Cipher Feedback REQUIREMENTS **
    uint8_t mEnIV[CAST5::BLOCK_SIZE]; //!< Encrypt IV (Zero-filled by default)
    uint8_t mDeIV[CAST5::BLOCK_SIZE]; //!< Decrypt IV (Zero-filled by default)
    int32_t mEnNum;                   //!< Encrypt IV Position (Equal to zero by default)
    int32_t mDeNum;                   //!< Decrypt IV Position (Equal to zero by default)
};

#endif // _COPS_V7_EMULATOR_CAST5_CFB64_H_
