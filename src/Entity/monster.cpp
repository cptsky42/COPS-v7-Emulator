/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "monster.h"
#include "world.h"
#include "generator.h"
#include "player.h"
#include "msgplayer.h"

Monster :: Monster(uint32_t aUID, const Monster::Info& aInfo, Generator* aOwner)
    : AdvancedEntity(aUID), mInfo(aInfo), mOwner(aOwner)
{
    if (mOwner != nullptr)
        ++mOwner->mAmount;

    mName = aInfo.Name;
    mLook = aInfo.Look;

    mLevel = aInfo.Level;

    mCurHP = aInfo.Life;
    mMaxHP = aInfo.Life;

    mPose = AdvancedEntity::POSE_STANDBY;
}

Monster :: ~Monster()
{
    static World& world = World::getInstance();
    world.recycleMonsterUID(mUID);
}

void
Monster :: sendShow(const Player& aPlayer) const
{
    MsgPlayer msg(*this);
    aPlayer.send(&msg);
}


// TODO die method and decrement mAmount & mGenAmount
// if mGenAmount < mAmount => error & mGenAmount = mAmount
