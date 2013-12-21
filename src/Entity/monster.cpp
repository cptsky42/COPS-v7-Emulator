/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "monster.h"
#include "player.h"
#include "msgplayer.h"

Monster :: Monster(uint32_t aUID, Monster::Info& aInfo)
    : AdvancedEntity(aUID)
{
    mId = aInfo.Id;
    mName = aInfo.Name;
    mType = aInfo.Type;
    mLook = aInfo.Look;

    mCurHP = aInfo.Life;
    mMaxHP = aInfo.Life;
    mEscapeLife = aInfo.EscapeLife;

    mLevel = aInfo.Level;

    mMinAtk = aInfo.MinAtk;
    mMaxAtk = aInfo.MaxAtk;
    mDefense = aInfo.Defense;
    mDexterity = aInfo.Dexterity;
    mDodge = aInfo.Dodge;

    mViewRange = aInfo.ViewRange;
    mAtkSpeed = aInfo.AtkSpeed;
    mMoveSpeed = aInfo.MoveSpeed;

    mDefy = aInfo.Defy;
}

Monster :: ~Monster()
{

}

void
Monster :: sendShow(const Player& aPlayer) const
{
    MsgPlayer msg(*this);
    aPlayer.send(&msg);
}
