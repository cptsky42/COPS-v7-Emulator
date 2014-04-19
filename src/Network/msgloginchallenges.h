/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_LOGIN_CHALLENGE_S_H_
#define _COPS_V7_EMULATOR_MSG_LOGIN_CHALLENGE_S_H_

#include "common.h"
#include "msg.h"

/**
 * Diffie-Hellman response to a request (B key). It will help to compute
 * the final shared S key.
 */
class MsgLoginChallengeS : public Msg
{
public:
    /** The number of bytes of the padding. */
    static const size_t PADDING_LEN = 7;

public:
    /* Unused internally ! */
    #pragma pack(push, 1)
    typedef struct
    {
        /** Junk padding ? */
        uint8_t Padding[MsgLoginChallengeS::PADDING_LEN];
        /** Size of the whole message, including the TQ seal. */
        int32_t Size;
        /** Size of the junk part. */
        int32_t JunkSize;
        /** Junk (random bytes) */
        uint8_t Junk[1];
        /** Size of the hex string representing the B key. */
        int32_t BSize;
        /** B key of the DH exchange */
        char B[1];
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a message object from the specified buffer.
     * The buffer will be took by the object and the memory
     * freed when the object will be destroyed.
     *
     * If the server is on a Be architecture, all the integers
     * are swapped.
     *
     * @param[in,out] aBuf        a pointer to the buffer to take
     *                            the pointer will be set to null
     * @param[in]     aLen        the length in bytes of the buffer
     */
    MsgLoginChallengeS(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgLoginChallengeS();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient         a pointer to the client which
     *                                has sent the message
     * @param[in]     aEncryptIV      the encryption IV
     * @param[in]     aDecryptIV      the decryption IV
     */
    void process(Client* aClient,
                 const uint8_t* aEncryptIV, const uint8_t* aDecryptIV);

    using Msg::process; // do not hide the virtual function...

private:
    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;
};

#endif // _COPS_V7_EMULATOR_MSG_LOGIN_CHALLENGE_S_H_
