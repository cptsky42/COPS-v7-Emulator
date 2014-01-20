/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgmapinfo.h"
#include "gamemap.h"

MsgMapInfo :: MsgMapInfo(const GameMap& aMap)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aMap);
}

MsgMapInfo :: ~MsgMapInfo()
{

}

void
MsgMapInfo :: create(const GameMap& aMap)
{
    ASSERT(&aMap != nullptr);

    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_MAPINFO;

    mInfo->UniqId = aMap.getUID();
    mInfo->DocId = aMap.getDocID();
    mInfo->Type = aMap.getType();
}

void
MsgMapInfo :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->UniqId = bswap32(info->UniqId);
    info->DocId = bswap32(info->DocId);
    info->Type = bswap32(info->Type);
}

