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

        Item* equip = nullptr;
        mInfo->GarmentType = (equip = aPlayer.getEquipByPos(Item::POS_GARMENT)) != nullptr ? equip->getType() : 0;
        mInfo->ArmetType = (equip = aPlayer.getEquipByPos(Item::POS_ARMET)) != nullptr ? equip->getType() : 0;
        mInfo->ArmorType = (equip = aPlayer.getEquipByPos(Item::POS_ARMOR)) != nullptr ? equip->getType() : 0;
        mInfo->WeaponRType = (equip = aPlayer.getEquipByPos(Item::POS_RWEAPON)) != nullptr ? equip->getType() : 0;
        mInfo->WeaponLType = (equip = aPlayer.getEquipByPos(Item::POS_LWEAPON)) != nullptr ? equip->getType() : 0;

        memset(mInfo->Padding1, 0, sizeof(mInfo->Padding1));

        mInfo->Life = 0;
        mInfo->Level = 0;

        mInfo->PosX = aPlayer.getPosX();
        mInfo->PosY = aPlayer.getPosY();

        mInfo->Hair = aPlayer.getHair();

        mInfo->Direction = aPlayer.getDirection();
        mInfo->Pose = aPlayer.getPose();

        mInfo->Metempsychosis = aPlayer.getMetempsychosis();
        mInfo->PlayerLevel = aPlayer.getLevel();

        memset(mInfo->Padding2, 0, sizeof(mInfo->Padding2));

        mInfo->NobilityRank = 0; // TODO implement me
        mInfo->NobilityUID = aPlayer.getUID();
        mInfo->NobilityPos = 0; // TODO implement me

        memset(mInfo->Padding3, 0, sizeof(mInfo->Padding3));

        mInfo->ArmetColor = (equip = aPlayer.getEquipByPos(Item::POS_ARMET)) != nullptr ? equip->getColor() : 0;
        mInfo->ArmorColor = (equip = aPlayer.getEquipByPos(Item::POS_ARMOR)) != nullptr ? equip->getColor() : 0;

        memset(mInfo->Padding4, 0, sizeof(mInfo->Padding4));

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

        mInfo->GarmentType = 0;
        mInfo->ArmetType = 0;
        mInfo->ArmorType = 0;
        mInfo->WeaponRType = 0;
        mInfo->WeaponLType = 0;

        memset(mInfo->Padding1, 0, sizeof(mInfo->Padding1));

        mInfo->Life = aMonster.getCurHP();
        mInfo->Level = aMonster.getLevel();

        mInfo->PosX = aMonster.getPosX();
        mInfo->PosY = aMonster.getPosY();

        mInfo->Hair = 0;

        mInfo->Direction = aMonster.getDirection();
        mInfo->Pose = aMonster.getPose();

        mInfo->Metempsychosis = 0;
        mInfo->PlayerLevel = 0;

        memset(mInfo->Padding2, 0, sizeof(mInfo->Padding2));
        mInfo->NobilityRank = 0;
        mInfo->NobilityUID = 0;
        mInfo->NobilityPos = 0;
        memset(mInfo->Padding3, 0, sizeof(mInfo->Padding3));
        mInfo->ArmetColor = 0;
        mInfo->ArmorColor = 0;
        memset(mInfo->Padding4, 0, sizeof(mInfo->Padding4));

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
    info->ArmetType = bswap32(info->ArmetType);
    info->ArmorType = bswap32(info->ArmorType);
    info->WeaponRType = bswap32(info->WeaponRType);
    info->WeaponLType = bswap32(info->WeaponLType);

    info->PosX = bswap16(info->PosX);
    info->PosY = bswap16(info->PosY);
    info->Hair = bswap16(info->Hair);

    info->Metempsychosis = bswap16(info->Metempsychosis);
    info->PlayerLevel = bswap16(info->PlayerLevel);

    info->NobilityRank = bswap32(info->NobilityRank);
    info->NobilityUID = bswap32(info->NobilityUID);
    info->NobilityPos = bswap32(info->NobilityPos);

    info->ArmetColor = bswap16(info->ArmetColor);
    info->ArmorColor = bswap16(info->ArmorColor);

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
