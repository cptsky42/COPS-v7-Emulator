/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "advancedentity.h"

AdvancedEntity :: AdvancedEntity(uint32_t aUID)
    : Entity(aUID)
{
    mName = "Unknown";

    mLevel = 1;

    mCurHP = 0;
    mMaxHP = 0;

    mPose = POSE_STANDBY;
}

AdvancedEntity :: ~AdvancedEntity()
{

}
