/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgflushexp.h"

MsgFlushExp :: MsgFlushExp(uint16_t aType, uint32_t aExp, Action aAction)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aType, aExp, aAction);
}

MsgFlushExp :: ~MsgFlushExp()
{

}

void
MsgFlushExp :: create(uint16_t aType, uint32_t aExp, Action aAction)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_FLUSHEXP;

    mInfo->Type = aType;
    mInfo->Exp = aExp;
    mInfo->Action = (uint8_t)aAction;
}

void
MsgFlushExp :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Type = bswap16(info->Type);
    info->Exp = bswap32(info->Exp);
}

