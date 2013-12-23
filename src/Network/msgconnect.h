/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_CONNECT_H_
#define _COPS_V7_EMULATOR_MSG_CONNECT_H_

#include "common.h"
#include "msg.h"

/**
 * Msg sent by the AccServer to answer to a connection request.
 */
class MsgConnect : public Msg
{
public:
    static const int32_t INVALID_UID = 0;
    static const char ERROR_SERVER_DOWN[];
    static const char ERROR_INVALID_ACC[];
    static const char ERROR_TRY_LATER[];

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** THe account UID (key B) */
        int32_t AccountUID;
        /** The token / session ID of the connection (key A) */
        int32_t Data;
        /** The information (IP address) of the game server */
        char Info[MAX_NAMESIZE];
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgConnect for the specified account.
     *
     * @param aAccUID[in]   the account UID
     * @param aData[in]     the session ID
     * @param aInfo[in]     the game server IP address
     */
    MsgConnect(int32_t aAccUID, int32_t aData, const char* aInfo);

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
    MsgConnect(uint8_t** aBuf, size_t aLen);

    /* destructor */
    ~MsgConnect();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

private:
    /* internal filling of the packet */
    void create(int32_t aAccUID, int32_t aData, const char* aInfo);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_CONNECT_H_
