/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgplayer.h"
#include "stringpacker.h"
#include "player.h"
#include "monster.h"
#include <string.h>

MsgPlayer :: MsgPlayer(const Player& aPlayer)
    : Msg(sizeof(MsgInfo) +
          strlen(aPlayer.getName()) + 1)
{
    mInfo = (MsgInfo*)mBuf;
    create(aPlayer);
}

MsgPlayer :: MsgPlayer(const Monster& aMonster)
    : Msg(sizeof(MsgInfo) +
          strlen(aMonster.getName()) + 1),
      mInfo((MsgInfo*)mBuf)
{
    create(aMonster);
}

MsgPlayer :: MsgPlayer(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}

MsgPlayer :: ~MsgPlayer()
{

}

void
MsgPlayer :: create(const Player& aPlayer)
{
    ASSERT(&aPlayer != nullptr);
    ASSERT(aPlayer.getName() != nullptr && aPlayer.getName()[0] != '\0');

    if (strlen(aPlayer.getName()) < MAX_NAMESIZE)
    {
        mInfo->Header.Length = mLen;
        mInfo->Header.Type = MSG_PLAYER;

        mInfo->UniqId = aPlayer.getUID();
        mInfo->Look = aPlayer.getLook();

        mInfo->Status = 0; // TODO: Implement me
        mInfo->SynId_Rank = 0; // TODO: (aPlayer.getSynRank() << MASK_RANK_SHIFT) | (aPlayer.getSynId() & MASK_SYNID);

        /*
        mInfo->WeaponRType = 0; // TODO: Implement me
        mInfo->WeaponLType = 0; // TODO: Implement me
        mInfo->MountType = 0; // TODO: Implement me
        mInfo->MantleType = 0; // TODO: Implement me
        */

        mInfo->PosX = aPlayer.getPosX();
        mInfo->PosY = aPlayer.getPosY();

        mInfo->Hair = aPlayer.getHair();

        mInfo->Direction = aPlayer.getDirection();
        mInfo->Pose = aPlayer.getPose();

        StringPacker packer(mInfo->Buf);
        packer.addString(aPlayer.getName());
    }
    else
    {
        LOG(ERROR, "Invalid length: name=%zu",
            strlen(aPlayer.getName()));
    }
}

void
MsgPlayer :: create(const Monster& aMonster)
{
    ASSERT(&aMonster != nullptr);
    ASSERT(aMonster.getName() != nullptr && aMonster.getName()[0] != '\0');

    if (strlen(aMonster.getName()) < MAX_NAMESIZE)
    {
        mInfo->Header.Length = mLen;
        mInfo->Header.Type = MSG_PLAYER;

        mInfo->UniqId = aMonster.getUID();
        mInfo->Look = aMonster.getLook();

        mInfo->Status = 0; // TODO: Implement me
        mInfo->OwnerUID = 0; // TODO: Pet

        /* TODO
        mInfo->WeaponRType = 0;
        mInfo->WeaponLType = 0;
        mInfo->MountType = 0;
        mInfo->MantleType = 0;
        */

        mInfo->Life = aMonster.getCurHP();
        mInfo->Level = aMonster.getLevel();

        mInfo->PosX = aMonster.getPosX();
        mInfo->PosY = aMonster.getPosY();

        mInfo->Hair = 0;

        mInfo->Direction = aMonster.getDirection();
        mInfo->Pose = aMonster.getPose();

        StringPacker packer(mInfo->Buf);
        packer.addString(aMonster.getName());
    }
    else
    {
        LOG(ERROR, "Invalid length: name=%zu",
            strlen(aMonster.getName()));
    }
}

void
MsgPlayer :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    int uid = info->UniqId;

    info->UniqId = bswap32(info->UniqId);
    info->Look = bswap32(info->Look);
    info->Status = bswap64(info->Status);

    info->GarmentType = bswap32(info->GarmentType);
    info->HeadType = bswap32(info->HeadType);
    info->ArmorType = bswap32(info->ArmorType);
    info->WeaponRType = bswap32(info->WeaponRType);
    info->WeaponLType = bswap32(info->WeaponLType);

    info->PosX = bswap16(info->PosX);
    info->PosY = bswap16(info->PosY);
    info->Hair = bswap16(info->Hair);
    info->Pose = bswap16(info->Pose);
    if (Entity::isPlayer(uid))
    {
        info->SynId_Rank = bswap32(info->SynId_Rank);
    }
    else if (Entity::isMonster(uid))
    {
        info->OwnerUID = bswap32(info->OwnerUID);
        info->Life = bswap16(info->Life);
        info->Level = bswap16(info->Level);
    }
    else
    {
        /// unknown UID
        ASSERT(false);
    }
}
