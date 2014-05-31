/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgmagicinfo.h"

MsgMagicInfo :: MsgMagicInfo(uint16_t aType, uint8_t aLevel, uint32_t aExp)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aType, aLevel, aExp);
}

MsgMagicInfo :: ~MsgMagicInfo()
{

}

void
MsgMagicInfo :: create(uint16_t aType, uint8_t aLevel, uint32_t aExp)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_MAGICINFO;

    mInfo->Type = aType;
    mInfo->Level = aLevel;
    mInfo->Exp = aExp;
}

void
MsgMagicInfo :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Type = bswap16(info->Type);
    info->Level = bswap16(info->Level);
    info->Exp = bswap32(info->Exp);
}

