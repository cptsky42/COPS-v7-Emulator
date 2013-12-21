/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_ADVANCED_ENTITY_H
#define _COPS_V7_EMULATOR_ADVANCED_ENTITY_H

#include "common.h"
#include "entity.h"

class Player;

/**
 * Base class of all advanced entity on a map.
 * An advanced entity can be in a battle.
 */
class AdvancedEntity : public Entity
{
    PROHIBIT_COPY(AdvancedEntity); // constant UID, should be unique...

public:
    static const uint16_t POSE_DANCE1       =   1;
    static const uint16_t POSE_STANDBY      = 100;
    static const uint16_t POSE_LAUGH        = 150;
    static const uint16_t POSE_GUFFAW       = 151;
    static const uint16_t POSE_FURY         = 160;
    static const uint16_t POSE_SAD          = 170;
    static const uint16_t POSE_EXCITEMENT   = 180;
    static const uint16_t POSE_SAYHELLO     = 190;
    static const uint16_t POSE_SALUTE       = 200;
    static const uint16_t POSE_GENUFLECT    = 210;
    static const uint16_t POSE_KNEEL        = 220;
    static const uint16_t POSE_COOL         = 230;
    static const uint16_t POSE_SWIM         = 240;
    static const uint16_t POSE_SITDOWN      = 250;
    static const uint16_t POSE_ZAZEN        = 260;
    static const uint16_t POSE_FAINT        = 270;
    static const uint16_t POSE_LIE          = 271;

public:
    /* destructor */
    virtual ~AdvancedEntity();

public:
    /** Send the entity spawn msg. */
    virtual void sendShow(const Player& aPlayer) const = 0;

    /** Called when the timer elapse. */
    virtual void timerElapsed(time_t aTime) = 0;

public:
    /** Get the entity's level. */
    uint8_t getLevel() const { return mLevel; }

    /** Get the entity's current hit points. */
    uint16_t getCurHP() const { return mCurHP; }
    /** Get the entity's max hit points. */
    uint16_t getMaxHP() const { return mMaxHP; }

    /** Get the entity's pose. */
    uint16_t getPose() const { return mPose; }

protected:
    /* constructor */
    AdvancedEntity(uint32_t aUID);

protected:
    uint8_t mLevel; //!< Entity level

    uint16_t mCurHP; //!< Entity current HP
    uint16_t mMaxHP; //!< Entity max HP

    int32_t mMinAtk; //!< Entity minimum attack
    int32_t mMaxAtk; //!< Entity maximum attack
    int32_t mDefense; //!< Entity defense
    int32_t mMAtk; //!< Entity magic attack
    int32_t mMDef; //!< Entity magic def
    uint8_t mDexterity; //!< Entity dexterity
    uint8_t mDodge; //!< Entity dodge

    uint16_t mPose; //!< Entity pose
};

#endif // _COPS_V7_EMULATOR_ADVANCED_ENTITY_H
