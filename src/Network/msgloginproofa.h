/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_LOGIN_PROOF_A_H_
#define _COPS_V7_EMULATOR_MSG_LOGIN_PROOF_A_H_

#include "common.h"
#include "msg.h"
#include "blowfish.h"
#include <string>

/**
 * Diffie-Hellman request exposing the public keys P & G and the other computed
 * public key A. Also, includes the new IVs of the Blowfish algorithm.
 */
class MsgLoginProofA : public Msg
{
public:
    /** The number of bytes of the padding. */
    static const size_t PADDING_LEN = 11;
    /** The number of bytes of the junk. */
    static const size_t JUNK_LEN = 12;

public:
    /* Unused internally ! */
    #pragma pack(push, 1)
    typedef struct
    {
        uint8_t Padding[MsgLoginProofA::PADDING_LEN];
        int32_t Size;
        int32_t JunkSize;
        uint8_t Junk[MsgLoginProofA::JUNK_LEN];
        int32_t EnIVSize;
        uint8_t EnIV[Blowfish::BLOCK_SIZE];
        int32_t DeIVSize;
        uint8_t DeIV[Blowfish::BLOCK_SIZE];
        int32_t PSize;
        char P[1];
        int32_t GSize;
        char G[1];
        int32_t ASize;
        char A[1];
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgLoginProofA packet for the specified client.
     *
     * @param[in]   aEncryptIV      the encryption IV
     * @param[in]   aDecryptIV      the decryption IV
     * @param[in]   aP              the P key
     * @param[in]   aG              the G key
     * @param[in]   aA              the A key
     */
    MsgLoginProofA(const uint8_t* aEncryptIV, const uint8_t* aDecryptIV,
                   const std::string& aP, const std::string& aG, const std::string& aA);

    /* destructor */
    virtual ~MsgLoginProofA();

private:
    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;
};

#endif // _COPS_V7_EMULATOR_MSG_LOGIN_PROOF_A_H_
