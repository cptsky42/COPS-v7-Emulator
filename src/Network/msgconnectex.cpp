/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgconnectex.h"
#include "client.h"
#include "player.h"
#include "database.h"
#include "world.h"
#include "msgtalk.h"
#include "msguserinfo.h"
#include "msguserattrib.h"
#include "msgiteminfo.h"
#include "msgtick.h"

/* static */
const char MsgConnectEx::ERROR_SERVER_DOWN[] = "\xB7\xFE\xCE\xF1\xC6\xF7\xCE\xB4\xC6\xF4\xB6\xAF";
const char MsgConnectEx::ERROR_INVALID_ACC[] = "\xD5\xCA\xBA\xC5\xC3\xFB\xBB\xF2\xBF\xDA\xC1\xEE\xB4\xED";
const char MsgConnectEx::ERROR_BANNED[] = "";
const char MsgConnectEx::ERROR_NOT_ENOUGH_CREDITS[] = "";
const char MsgConnectEx::ERROR_NOT_ENOUGH_GAMETIME[] = "";
const char MsgConnectEx::ERROR_UNKNOWN_SERVER[] = "";
const char MsgConnectEx::ERROR_INVALID_PWD[] = "";
const char MsgConnectEx::ERROR_SERVER_BUSY[] = "";
const char MsgConnectEx::ERROR_SERVER_FULL[] = "";
const char MsgConnectEx::ERROR_DEFAULT[] = "";

MsgConnectEx :: MsgConnectEx(int32_t aAccUID, int32_t aData, const char* aInfo, uint16_t aPort)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aAccUID, aData, aInfo, aPort);
}

MsgConnectEx :: MsgConnectEx(int32_t aData, const char* aInfo)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(MsgConnectEx::INVALID_UID, aData, aInfo, 0);
}

MsgConnectEx :: ~MsgConnectEx()
{

}

void
MsgConnectEx :: create(int32_t aAccUID, int32_t aData, const char* aInfo, uint16_t aPort)
{
    ASSERT(aInfo != nullptr);

    if (strlen(aInfo) < MAX_NAMESIZE)
    {
        mInfo->Header.Length = mLen;
        mInfo->Header.Type = MSG_CONNECTEX;

        mInfo->AccountUID = aAccUID;
        mInfo->Data = aData;
        strcpy(mInfo->Info, aInfo);
        mInfo->Port = aPort;
    }
    else
    {
        LOG(ERROR, "Invalid length: info=%zu", strlen(aInfo));
    }
}

void
MsgConnectEx :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->AccountUID = bswap32(info->AccountUID);
    info->Data = bswap32(info->Data);
    info->Port = bswap32(info->Port);
}
