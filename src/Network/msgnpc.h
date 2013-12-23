/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_NPC_H_
#define _COPS_V7_EMULATOR_MSG_NPC_H_

#include "common.h"
#include "msg.h"

class MsgNpc : public Msg
{
public:
    enum Event
    {
        EVENT_BEACTIVED = 0, // to server
        EVENT_ADDNPC = 1, // no use
        EVENT_LEAVEMAP = 2, // to client
        EVENT_DELNPC = 3, // to server
        EVENT_CHANGEPOS = 4,
        EVENT_LAYNPC = 5 // to client(id=region,data=lookface), answer MsgNpcInfo(CMsgPlayer for statuary)
    };

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        uint32_t Id;

        union
        {
            uint32_t Data;
            struct
            {
                uint16_t PosX;
                uint16_t PosY;
            };
        };

        uint16_t Event;
        uint16_t Type;
    }MsgInfo;
    #pragma pack(pop)

public:
    MsgNpc(uint32_t aId, uint32_t aData, uint16_t aType, Event aEvent);

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
    MsgNpc(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgNpc();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

private:
    /* internal filling of the packet */
    void create(uint32_t aId, uint32_t aData, uint16_t aType, Event aEvent);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_NPC_H_
