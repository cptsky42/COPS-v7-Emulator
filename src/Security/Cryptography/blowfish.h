/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 * Copyright (C) 1993 Bruce Schneier
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_BLOWFISH_CFB64_H_
#define _COPS_V7_EMULATOR_BLOWFISH_CFB64_H_

#include "common.h"

/**
 * Blowfish is a keyed, symmetric block cipher, designed in 1993 by
 * Bruce Schneier. Blowfish is unpatented. The algorithm is placed
 * in the public domain, and can be freely used by anyone.
 *
 * Blowfish has a 64-bit block size and a variable key length
 * from 1 bit up to 448 bits. It is a 16-round Feistel cipher and
 * uses large key-dependent S-boxes.
 *
 * This implementation uses the Cipher Feedback 64 mode on stream.
 * This implementation requires 4 KiO of memory.
 */
class Blowfish
{
public:
    /** The block size of the algorithm in bits (must be 64) */
    static const int32_t BLOCK_SIZE = INT32_C(8);
    /** The number of round of the algorithm (must be 16) */
    static const int32_t ROUNDS = INT32_C(16);
    /** The key size of the algorithm in bits (1 to 448) */
    static const int32_t KEY_SIZE = INT32_C(56);

public:
    /* constructor */
    Blowfish();

    /* destructor */
    ~Blowfish();

    /**
     * Generates the mixed P-Array and S-Boxes based on the given seed.
     * Positions of the IVs will be set to zero.
     *
     * @param[in]     aSeed         the seed that will be used
     * @param[in]     aLen          the number of octets of the seed
     */
    void generateKey(uint8_t* aSeed, size_t aLen);

    /**
     * Encrypts n octet(s) with the Blowfish algorithm and the CFB64 mode.
     *
     *
     * @param[in,out] aBuf          the buffer that will be encrypted
     * @param[in]     aLen          the number of octets to encrypt
     */
    void encrypt(uint8_t* aBuf, size_t aLen);

    /**
     * Encrypts n octet(s) with the Blowfish algorithm and the CFB64 mode.
     *
     *
     * @param[in,out] aBuf          the buffer that will be encrypted
     * @param[in]     aLen          the number of octets to encrypt
     */
    void decrypt(uint8_t* pBuf, size_t aLen);

    /**
     * Sets the encryption IV and/or the decryption IV with the given value.
     * This function is specific to the CFB64 mode.
     *
     * @param[in]     aEnvIV        the new encryption IV
     * @param[in]     aDecIV        the new decryption IV
     */
    void setIVs(uint8_t aEncIV[Blowfish::BLOCK_SIZE],
                uint8_t aDecIV[Blowfish::BLOCK_SIZE]);

private:
    /**
     * F is a Blowfish function which returns 32-bit unsigned integer generated from
     * the S-Boxes and the seed.
     *
     * @param[in]     aSeed         the seed that will be used
     *
     * @retval A 32-bit unsigned integer generated from the S-Boxes and the seed.
     */
    inline uint32_t F(uint32_t aSeed)
    {
        return ((mS[0][(aSeed >> 24) & 0xFF] + mS[1][(aSeed >> 16) & 0xFF])
                ^ mS[2][(aSeed >> 8) & 0xFF]) + mS[3][(aSeed) & 0xFF];
    }

    /**
     * Encrypts a block with the Blowfish algorithm.
     *
     * @param[in,out] aBlock        the block to be encrypted
     */
    void encipher(uint8_t aBlock[Blowfish::BLOCK_SIZE]);

    /**
     * Decrypts a block with the Blowfish algorithm.
     *
     * @param[in,out] aBlock        the block to be decrypted
     */
    void decipher(uint8_t aBlock[Blowfish::BLOCK_SIZE]);

private:
    static const uint32_t P[Blowfish::ROUNDS + 2]; //!< Blowfish P-Array
    static const uint32_t S[4][256];               //!< Blowfish S-Boxes

    uint32_t mP[Blowfish::ROUNDS + 2];            //!< Mixed P-Array
    uint32_t mS[4][256];                          //!< Mixed S-Boxes

    //** Cipher Feedback REQUIREMENTS **
    uint8_t mEnIV[Blowfish::BLOCK_SIZE]; //!< Encrypt IV (Zero-filled by default)
    uint8_t mDeIV[Blowfish::BLOCK_SIZE]; //!< Decrypt IV (Zero-filled by default)
    int32_t mEnNum;                      //!< Encrypt IV Position (Equal to zero by default)
    int32_t mDeNum;                      //!< Decrypt IV Position (Equal to zero by default)
};

#endif // _COPS_V7_EMULATOR_BLOWFISH_CFB64_H_
