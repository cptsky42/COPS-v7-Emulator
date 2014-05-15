/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_CONNECTEX_H_
#define _COPS_V7_EMULATOR_MSG_CONNECTEX_H_

#include "common.h"
#include "msg.h"

/**
 * Msg sent by the AccServer to answer to a connection request.
 */
class MsgConnectEx : public Msg
{
public:
    static const int32_t INVALID_UID = 0;

    static const int32_t ERROR_ID_INVALID_PWD = 1; // 10418
    static const int32_t ERROR_ID_SERVER_DOWN = 10; // 10417
    static const int32_t ERROR_ID_LOGIN_LATER = 11; // 10419
    static const int32_t ERROR_ID_BANNED = 12; // 10438
    static const int32_t ERROR_ID_SERVER_BUSY = 20; // 10480
    static const int32_t ERROR_ID_SERVER_FULL = 21; // 10480
    static const int32_t ERROR_ID_ACCOUNT_LOCKED = 22; // 10440
    static const int32_t ERROR_ID_UNKNOWN_ERROR = 999; // 10439

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** THe account UID */
        int32_t AccountUID;
        /** The token / session ID of the connection  */
        int32_t Data;
        /** The information (IP address) of the MsgServer or the error string */
        char Info[MAX_NAMESIZE];
        /** The port of the MsgServer. */
        uint32_t Port;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgConnect for the specified account.
     *
     * @param aAccUID[in]   the account UID
     * @param aData[in]     the session ID
     * @param aInfo[in]     the game server IP address
     * @param aPort[in]     the game server port
     */
    MsgConnectEx(int32_t aAccUID, int32_t aData, const char* aInfo, uint16_t aPort);

    /**
     * Create a new MsgConnect for the specified account.
     *
     * @param aErrorId[in]     the error ID
     */
    MsgConnectEx(int32_t aErrorId);

    /* destructor */
    ~MsgConnectEx();

private:
    /* internal filling of the packet */
    void create(int32_t aAccUID, int32_t aData, const char* aInfo, uint16_t aPort);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_CONNECTEX_H_
