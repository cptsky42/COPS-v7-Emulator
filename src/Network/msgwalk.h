/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_WALK_H_
#define _COPS_V7_EMULATOR_MSG_WALK_H_

#include "common.h"
#include "msg.h"

/**
 * Msg sent to the client by the MsgServer or by the client to the MsgServer to
 * indicate a deplacement in a specific direction by walking or running.
 */
class MsgWalk : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** The unique Id of the entity which is walking */
        uint32_t UniqId;
        /** The direction of the mouvement */
        uint8_t Direction;
        /** The mode of the mouvement (walk/run) */
        uint8_t Mode;
        /** Unknown bytes (padding ?) */
        int16_t Padding1;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a MsgWalk packet for the specified entity to move it
     * in the specified direction.
     *
     * @param[in]  aUniqId      the unique Id of the entity
     * @param[in]  aDirection   the direction of the mouvement
     * @param[in]  aIsRunning   (optional) the mode of the mouvement
     */
    MsgWalk(int32_t aUniqId, uint8_t aDirection, bool aIsRunning = false);

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
    MsgWalk(uint8_t** aBuf, size_t aLen);

    /* destructor */
    ~MsgWalk();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

private:
    /* internal filling of the packet */
    void create(int32_t aUniqId, uint8_t aDirection, bool aIsRunning);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_WALK_H_
