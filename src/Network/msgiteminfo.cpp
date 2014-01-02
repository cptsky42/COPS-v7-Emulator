/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgiteminfo.h"

MsgItemInfo :: MsgItemInfo(void* aItem, Action aAction)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aItem, aAction);
}

MsgItemInfo :: MsgItemInfo(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgItemInfo :: ~MsgItemInfo()
{

}

void
MsgItemInfo :: create(void* aItem, Action aAction)
{
    //ASSERT(aItem != nullptr);

    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_ITEMINFO;

    mInfo->UniqId = 1000000;
    mInfo->Type = ((int*)aItem)[0];
    mInfo->Amount = 1099;
    mInfo->AmountLimit = 10099;
    mInfo->Action = (uint8_t)aAction;
    mInfo->Ident = 0;
    mInfo->Position = ((int*)aItem)[1];
    memset(mInfo->Padding, 0, sizeof(mInfo->Padding));
    mInfo->Gem1 = 13;
    mInfo->Gem2 = 23;
    mInfo->Magic1 = 201;
    mInfo->Magic2 = 0;
    mInfo->Magic3 = 9; // Ph-Def Bonus
    mInfo->Bless = 7;
    mInfo->Enchant = 255;
    mInfo->Restrain = 2;
    mInfo->Locked = true;
    mInfo->Suspicious = true;
    mInfo->Color = 4;
}

void
MsgItemInfo :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->UniqId = bswap32(info->UniqId);
    info->Type = bswap32(info->Type);
    info->Amount = bswap16(info->Amount);
    info->AmountLimit = bswap16(info->AmountLimit);
    info->Restrain = bswap32(info->Restrain);
    info->Color = bswap32(info->Color);
}
