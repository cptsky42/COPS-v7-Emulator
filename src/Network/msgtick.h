/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_TICK_H_
#define _COPS_V7_EMULATOR_MSG_TICK_H_

#include "common.h"
#include "msg.h"

class Player;

/**
 * Message used to validate the client connection.
 */
class MsgTick : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** Unique Id of the entity */
        uint32_t UniqId;
        /** Timestamp of the creation of the packet */
        int32_t Timestamp;
        /** Random values. */
        int32_t Junk[4];
        /** Hash of the name  */
        uint32_t CheckData;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgTick packet for the specified player.
     *
     * @param[in]   aEntity     the entity doing the action
     */
    MsgTick(const Player& aPlayer);

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
    MsgTick(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgTick();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

private:
    /* internal filling of the packet */
    void create(const Player& aPlayer);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_TICK_H_
