/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msguserinfo.h"
#include "stringpacker.h"
#include "player.h"
#include <string.h>

MsgUserInfo :: MsgUserInfo(const Player& aPlayer)
    : Msg(sizeof(MsgInfo) +
          strlen(aPlayer.getName()) + 1 +
          strlen(aPlayer.getMate()) + 1),
      mInfo((MsgInfo*)mBuf)
{
    create(aPlayer);
}

MsgUserInfo :: ~MsgUserInfo()
{

}

void
MsgUserInfo :: create(const Player& aPlayer)
{
    ASSERT(&aPlayer != nullptr);
    ASSERT(aPlayer.getName() != nullptr && aPlayer.getName()[0] != '\0');
    ASSERT(aPlayer.getMate() != nullptr && aPlayer.getMate()[0] != '\0');

    if (strlen(aPlayer.getName()) < MAX_NAMESIZE &&
        strlen(aPlayer.getMate()) < MAX_NAMESIZE)
    {
        mInfo->Header.Length = mLen;
        mInfo->Header.Type = MSG_USERINFO;

        mInfo->UniqId = aPlayer.getUID();
        mInfo->Look = aPlayer.getLook();
        mInfo->Hair = aPlayer.getHair();
        mInfo->Money = aPlayer.getMoney();
        mInfo->ConquerPoints = aPlayer.getCPs();
        mInfo->Exp = aPlayer.getExp();
        mInfo->Force = aPlayer.getForce();
        mInfo->Health = aPlayer.getHealth();
        mInfo->Dexterity = aPlayer.getDexterity();
        mInfo->Soul = aPlayer.getSoul();
        mInfo->AddPoints = aPlayer.getAddPoints();
        mInfo->CurHP = aPlayer.getCurHP();
        mInfo->CurMP = aPlayer.getCurMP();
        mInfo->PkPoints = aPlayer.getPkPoints();
        mInfo->Level = aPlayer.getLevel();
        mInfo->Profession = aPlayer.getProfession();
        mInfo->AutoAllot = 1; // not really used by the client...
        mInfo->Metempsychosis = aPlayer.getMetempsychosis();
        mInfo->ShowName = 1;

        memset(mInfo->Padding, 0, sizeof(mInfo->Padding));

        StringPacker packer(mInfo->Buf);
        packer.addString(aPlayer.getName());
        packer.addString(aPlayer.getMate());
    }
    else
    {
        LOG(ERROR, "Invalid length: name=%zu, mate=%zu",
            strlen(aPlayer.getName()), strlen(aPlayer.getMate()));
    }
}

void
MsgUserInfo :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->UniqId = bswap32(info->UniqId);
    info->Look = bswap32(info->Look);
    info->Hair = bswap16(info->Hair);
    info->Money = bswap32(info->Money);
    info->ConquerPoints = bswap32(info->ConquerPoints);
    info->Exp = bswap64(info->Exp);
    info->Force = bswap16(info->Force);
    info->Health = bswap16(info->Health);
    info->Dexterity = bswap16(info->Dexterity);
    info->Soul = bswap16(info->Soul);
    info->AddPoints = bswap16(info->AddPoints);
    info->CurHP = bswap16(info->CurHP);
    info->CurMP = bswap16(info->CurMP);
    info->PkPoints = bswap16(info->PkPoints);
}
