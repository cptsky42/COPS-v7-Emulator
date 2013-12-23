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
    /* Unused internally ! */
    #pragma pack(push, 1)
    typedef struct
    {
        uint8_t Padding[7];
        int32_t Size;
        int32_t JunkSize;
        uint8_t Junk[1];
        int32_t BSize;
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
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

private:
    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;
};

#endif // _COPS_V7_EMULATOR_MSG_LOGIN_CHALLENGE_S_H_
