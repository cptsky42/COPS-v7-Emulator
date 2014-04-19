/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msguserattrib.h"
#include "client.h"
#include "entity.h"

MsgUserAttrib :: MsgUserAttrib(Entity* aEntity, uint64_t aData, UserAttrType aType)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aEntity, &aData, aType);
}

MsgUserAttrib :: ~MsgUserAttrib()
{

}

void
MsgUserAttrib :: create(Entity* aEntity, uint64_t* aData, UserAttrType aType)
{
    ASSERT(aEntity != nullptr);
    ASSERT(aData != nullptr);

    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_USERATTRIB;

    mInfo->UniqId = aEntity != nullptr ? aEntity->getUID() : 0;
    mInfo->Amount = 1;

    mInfo->Attrib[0].Type = (uint32_t)aType;
    mInfo->Attrib[0].Data = *aData;
}

void
MsgUserAttrib :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    for (uint32_t i = 0; i < info->Amount; ++i)
    {
        info->Attrib[i].Type = bswap32(info->Attrib[i].Type);
        info->Attrib[i].Data = bswap64(info->Attrib[i].Data);
    }
    info->UniqId = bswap32(info->UniqId);
    info->Amount = bswap32(info->Amount);
}
