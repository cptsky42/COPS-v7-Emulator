/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgweaponskill.h"
#include "weaponskill.h"

MsgWeaponSkill :: MsgWeaponSkill(const WeaponSkill& aSkill)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aSkill);
}

MsgWeaponSkill :: ~MsgWeaponSkill()
{

}

void
MsgWeaponSkill :: create(const WeaponSkill& aSkill)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_WEAPONSKILL;

    mInfo->Type = aSkill.getType();
    mInfo->Level = aSkill.getLevel();
    mInfo->Exp = aSkill.getExp();
}

void
MsgWeaponSkill :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Type = bswap32(info->Type);
    info->Level = bswap32(info->Level);
    info->Exp = bswap32(info->Exp);
}

