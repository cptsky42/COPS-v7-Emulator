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
        /** Junk padding ? */
        uint8_t Padding[MsgLoginProofA::PADDING_LEN];
        /** Size of the whole message, including the TQ seal. */
        int32_t Size;
        /** Size of the junk part. */
        int32_t JunkSize;
        /** Junk (random bytes). */
        uint8_t Junk[MsgLoginProofA::JUNK_LEN];
        /** Size of the encryption IV */
        int32_t EnIVSize;
        /** New encryption IV to be used. */
        uint8_t EnIV[Blowfish::BLOCK_SIZE];
        /** Size of the decryption IV */
        int32_t DeIVSize;
        /** New decryption IV to be used. */
        uint8_t DeIV[Blowfish::BLOCK_SIZE];
        /** Size of the p key. */
        int32_t PSize;
        /** p key used by the DH exchange. */
        char P[1];
        /** Size of the g key. */
        int32_t GSize;
        /** g key used by the DH exchange. */
        char G[1];
        /** Size of the A key. */
        int32_t ASize;
        /** A key used by the DH exchange. */
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
