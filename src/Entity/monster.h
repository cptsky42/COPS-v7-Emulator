/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MONSTER_H
#define _COPS_V7_EMULATOR_MONSTER_H

#include "common.h"
#include "advancedentity.h"
#include <string>

class Player;

/**
 * Base class of all monsters.
 */
class Monster : public AdvancedEntity
{
    PROHIBIT_COPY(Monster); // constant UID, should be unique...

public:
    /** The database informations for a monster ID. */
    struct Info
    {
    public:
        /** The ID of the monster. */
        int32_t Id;
        /** The name of the monster. */
        std::string Name;
        /** The type of the monster. */
        uint8_t Type;
        /** The look of the monster. */
        int32_t Look;
        /** The level of the monster. */
        uint8_t Level;
        /** The maximum life of the monster. */
        uint16_t Life;
        /** The escape life level of the monster. */
        uint16_t EscapeLife;
        /** The minimum attack of the monster. */
        int32_t MinAtk;
        /** The maximum attack of the monster. */
        int32_t MaxAtk;
        /** The defense of the monster. */
        int32_t Defense;
        /** The dexterity rate (%) of the monster. */
        uint8_t Dexterity;
        /** The dodge (%) of the monster. */
        uint8_t Dodge;
        /** The view range of the monster. */
        uint8_t ViewRange;
        /** The attack speed (in ms) of the monster. */
        uint16_t AtkSpeed;
        /** The move speed (in ms) of the monster. */
        uint16_t MoveSpeed;
        /** Determine wheter or not the monster defends people. */
        bool Defy;
    };

public:
    /* constructor */
    Monster(uint32_t aUID, Monster::Info& aInfo);

    /* destructor */
    virtual ~Monster();

public:
    /** Send the entity spawn msg. */
    virtual void sendShow(const Player& aPlayer) const;

    /** Called when the timer elapse. */
    virtual void timerElapsed(time_t aTime) { /* TODO */  }

private:
    int32_t mId; //!< Monster ID
    uint8_t mType; //!< Monster type
    uint16_t mEscapeLife; //!< Monster escape life

    uint8_t mViewRange; //!< Monster view range
    uint16_t mAtkSpeed; //!< Monster attack speed
    uint16_t mMoveSpeed; //!< Monster move speed

    bool mDefy; //!< Determine wheter or not the monster defends poeple
};

#endif // _COPS_V7_EMULATOR_MONSTER_H
