/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_WEAPON_SKILL_H_
#define _COPS_V7_EMULATOR_WEAPON_SKILL_H_

#include "common.h"

class Player;
class QSqlQuery;

/**
 * A weapon skill of a player.
 */
class WeaponSkill
{
public:
    /** Position of the SQL data in the result set. */
    enum SqlData
    {
        SQLDATA_ID = 0,
        SQLDATA_OWNER_ID,
        SQLDATA_TYPE,
        SQLDATA_LEVEL,
        SQLDATA_EXP,
        SQLDATA_OLD_LEVEL,
        SQLDATA_UNLEARN
    };

public:
    /**
     * Create a weapon skill object from a SQL query result set.
     *
     * @param[out]  aOutSkill  a pointer that will receive the object
     * @param[in]   aQuery     the SQL result set
     *
     * @retval ERROR_SUCCESS on success
     * @returns An error code otherwise
     */
    static err_t createSkill(WeaponSkill** aOutSkill, const QSqlQuery& aQuery);

public:
    /* destructor */
    ~WeaponSkill() { }

public:
    /** Get the weapon skill's unique ID. */
    uint32_t getUID() const { return mUID; }
    /** Get the weapon skill's type. */
    uint16_t getType() const { return mType; }

    /** Get the weapon skill's owner. */
    const Player& getPlayer() const { return mPlayer; }

    /** Get the weapon skill's level. */
    uint8_t getLevel() const { return mLevel; }
    /** Get the weapon skill's experience. */
    uint32_t getExp() const { return mExp; }
    /** Get the weapon skill's old level. */
    uint8_t getOldLevel() const { return mOldLevel; }
    /** Whether or not the weapon skill was unlearned. */
    bool isUnlearn() const { return mIsUnlearn; }

private:
    /* constructor */
    WeaponSkill(uint32_t aUID, uint16_t aType, const Player& aPlayer)
        : mUID(aUID), mType(aType), mPlayer(aPlayer),
          mLevel(0), mExp(0), mOldLevel(0), mIsUnlearn(false)
    {

    }

private:
    const uint32_t mUID; //!< the unique ID of the weapon skill
    const uint16_t mType; //!< the type of the weapon skill

    const Player& mPlayer; //!< the player owning the weapon skill

    uint8_t mLevel; //!< the level of the weapon skill
    uint32_t mExp; //!< the experience of the weapon skill
    uint8_t mOldLevel; //!< the previous level of the weapon skill (before reborn)
    bool mIsUnlearn; //!< whether or not the weapon skill was unlearn
};

#endif // _COPS_V7_EMULATOR_MAGIC_H_
