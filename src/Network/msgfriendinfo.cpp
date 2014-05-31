/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgfriendinfo.h"
#include "player.h"

MsgFriendInfo :: MsgFriendInfo(const Player& aPlayer)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aPlayer);
}

MsgFriendInfo :: ~MsgFriendInfo()
{

}

void
MsgFriendInfo :: create(const Player& aPlayer)
{
    ASSERT(&aPlayer != nullptr);
    ASSERT(aPlayer.getMate() != nullptr && aPlayer.getMate()[0] != '\0');

    if (strlen(aPlayer.getMate()) < MAX_NAMESIZE)
    {
        mInfo->Header.Length = mLen;
        mInfo->Header.Type = MSG_FRIENDINFO;

        // TODO HACK ! getSynRank() getSynId()
        mInfo->UniqId = aPlayer.getUID();
        mInfo->Look = aPlayer.getLook();
        mInfo->Level = aPlayer.getLevel();
        mInfo->Profession = aPlayer.getProfession();
        mInfo->PkPoints = aPlayer.getProfession();
        mInfo->SynId_Rank = (0 << MASK_RANK_SHIFT) | (0 & MASK_SYNID);
        strncpy(mInfo->Mate, aPlayer.getMate(), sizeof(mInfo->Mate));
    }
    else
    {
        LOG(ERROR, "Invalid length: mate=%zu",
            strlen(aPlayer.getMate()));
    }
}

void
MsgFriendInfo :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->UniqId = bswap32(info->UniqId);
    info->Look = bswap32(info->Look);
    info->PkPoints = bswap16(info->PkPoints);
    info->SynId_Rank = bswap32(info->SynId_Rank);
}

