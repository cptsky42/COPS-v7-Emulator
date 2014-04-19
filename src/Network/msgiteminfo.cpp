/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgiteminfo.h"
#include "item.h"

MsgItemInfo :: MsgItemInfo(const Item& aItem, Action aAction)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aItem, aAction);
}

MsgItemInfo :: ~MsgItemInfo()
{

}

void
MsgItemInfo :: create(const Item& aItem, Action aAction)
{
    ASSERT(&aItem != nullptr);

    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_ITEMINFO;

    mInfo->UniqId = aItem.getUID();
    mInfo->Type = aItem.getType();
    mInfo->Amount = aItem.getAmount();
    mInfo->AmountLimit = aItem.getAmountLimit();
    mInfo->Action = (uint8_t)aAction;
    mInfo->Ident = aItem.getIdent();
    mInfo->Position = (uint8_t)aItem.getPosition();
    memset(mInfo->Padding1, 0, sizeof(mInfo->Padding1));
    mInfo->Gem1 = aItem.getGem1();
    mInfo->Gem2 = aItem.getGem2();
    mInfo->Magic1 = aItem.getMagic1();
    mInfo->Magic2 = aItem.getMagic2();
    mInfo->Magic3 = aItem.getMagic3();
    mInfo->Bless = aItem.getBless();
    mInfo->Enchant = aItem.getEnchant();
    mInfo->Padding2 = 0;
    mInfo->Restrain = aItem.getRestrain();
    mInfo->Suspicious = aItem.isSuspicious();
    mInfo->Locked = aItem.isLocked();
    mInfo->Color = aItem.getColor();
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
    info->Suspicious = bswap16(info->Suspicious);
    info->Locked = bswap16(info->Locked);
    info->Color = bswap32(info->Color);
}
