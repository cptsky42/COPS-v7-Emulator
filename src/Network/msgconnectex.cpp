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

MsgConnectEx :: MsgConnectEx(int32_t aAccUID, int32_t aData, const char* aInfo, uint16_t aPort)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aAccUID, aData, aInfo, aPort);
}

MsgConnectEx :: MsgConnectEx(int32_t aErrorId)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(MsgConnectEx::INVALID_UID, aErrorId, "", 0);
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
