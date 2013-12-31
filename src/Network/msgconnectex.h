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

    static const int32_t ERROR_ID_SERVER_DOWN = 11;
    static const int32_t ERROR_ID_INVALID_ACC = 1;
    static const int32_t ERROR_ID_BANNED = 12;
    static const int32_t ERROR_ID_NOT_ENOUGH_CREDITS = 6;
    static const int32_t ERROR_ID_NOT_ENOUGH_GAMETIME = 7;
    static const int32_t ERROR_ID_UNKNOWN_SERVER = 10;
    static const int32_t ERROR_ID_INVALID_PWD = 13;
    static const int32_t ERROR_ID_SERVER_BUSY = 20;
    static const int32_t ERROR_ID_SERVER_FULL = 21;
    static const int32_t ERROR_ID_DEFAULT = 999;

    static const char ERROR_SERVER_DOWN[];
    static const char ERROR_INVALID_ACC[];
    static const char ERROR_BANNED[];
    static const char ERROR_NOT_ENOUGH_CREDITS[];
    static const char ERROR_NOT_ENOUGH_GAMETIME[];
    static const char ERROR_UNKNOWN_SERVER[];
    static const char ERROR_INVALID_PWD[];
    static const char ERROR_SERVER_BUSY[];
    static const char ERROR_SERVER_FULL[];
    static const char ERROR_DEFAULT[];

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
     * @param aData[in]     the error ID
     * @param aInfo[in]     the error string
     */
    MsgConnectEx(int32_t aData, const char* aInfo);

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
