/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgmagicinfo.h"
#include "magic.h"

MsgMagicInfo :: MsgMagicInfo(const Magic& aMagic)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aMagic);
}

MsgMagicInfo :: ~MsgMagicInfo()
{

}

void
MsgMagicInfo :: create(const Magic& aMagic)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_MAGICINFO;

    mInfo->Type = aMagic.getType();
    mInfo->Level = aMagic.getLevel();
    mInfo->Exp = aMagic.getExp();
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

