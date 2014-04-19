/*
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
class Generator;

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
        uint32_t Id;
        /** The name of the monster. */
        std::string Name;
        /** The type of the monster. */
        uint8_t Type;
        /** The AI type of the monster. */
        uint8_t AIType;
        /** The look of the monster. */
        uint32_t Look;
        /** The level of the monster. */
        uint8_t Level;
        /** The maximum life of the monster. */
        uint16_t Life;
        /** The escape life level of the monster. */
        uint16_t EscapeLife;
        /** Which users are attacked. */
        uint8_t AtkUser;
        /** The minimum attack of the monster. */
        uint32_t MinAtk;
        /** The maximum attack of the monster. */
        uint32_t MaxAtk;
        /** The defense of the monster. */
        uint32_t Defense;
        /** The dexterity rate (%) of the monster. */
        uint8_t Dexterity;
        /** The dodge (%) of the monster. */
        uint8_t Dodge;
        /** The magic skill used by the monster. */
        uint32_t MagicType;
        /** The magic defense of the monster. */
        uint32_t MDef;
        /** The magic hit rate of the monster. */
        uint32_t HitRate;
        /** The view range of the monster. */
        uint8_t ViewRange;
        /** The attack range of the monster. */
        uint8_t AtkRange;
        /** The attack speed (in ms) of the monster. */
        uint16_t AtkSpeed;
        /** The move speed (in ms) of the monster. */
        uint16_t MoveSpeed;
        /** The run speed (in ms) of the monster. */
        uint16_t RunSpeed;
        /** The armet dropped by the monster. */
        uint8_t DropArmet;
        /** The necklace dropped by the monster. */
        uint8_t DropNecklace;
        /** The armor dropped by the monster. */
        uint8_t DropArmor;
        /** The ring dropped by the monster. */
        uint8_t DropRing;
        /** The weapon dropped by the monster. */
        uint8_t DropWeapon;
        /** The shield dropped by the monster. */
        uint8_t DropShield;
        /** The shoes dropped by the monster. */
        uint8_t DropShoes;
        /** The money dropped by the monster. */
        uint32_t DropMoney;
        /** The HP dropped by the monster. */
        uint32_t DropHP;
        /** The MP dropped by the monster. */
        uint32_t DropMP;
        /** The battle level of the monster. */
        uint16_t ExtraBattleLvl;
        /** The extra exp of the monster. */
        uint16_t ExtraExp;
        /** The extra damage of the monster. */
        uint16_t ExtraDmg;
    };

    /** Position of the SQL data in the result set. */
    enum SqlData
    {
        SQLDATA_ID = 0,
        SQLDATA_NAME,
        SQLDATA_TYPE,
        SQLDATA_AI_TYPE,
        SQLDATA_LOOK,
        SQLDATA_LEVEL,
        SQLDATA_LIFE,
        SQLDATA_ESCAPE_LIFE,
        SQLDATA_ATK_USER,
        SQLDATA_MIN_ATK,
        SQLDATA_MAX_ATK,
        SQLDATA_DEFENSE,
        SQLDATA_DEXTERITY,
        SQLDATA_DODGE,
        SQLDATA_MAGIC_TYPE,
        SQLDATA_MAGIC_DEFENSE,
        SQLDATA_MAGIC_HIT_RATE,
        SQLDATA_VIEW_RANGE,
        SQLDATA_ATK_RANGE,
        SQLDATA_ATK_SPEED,
        SQLDATA_MOVE_SPEED,
        SQLDATA_RUN_SPEED,
        SQLDATA_DROP_ARMET,
        SQLDATA_DROP_NECKLACE,
        SQLDATA_DROP_ARMOR,
        SQLDATA_DROP_RING,
        SQLDATA_DROP_WEAPON,
        SQLDATA_DROP_SHIELD,
        SQLDATA_DROP_SHOES,
        SQLDATA_DROP_MONEY,
        SQLDATA_DROP_HP,
        SQLDATA_DROP_MP,
        SQLDATA_EXTRA_BATTLE_LVL,
        SQLDATA_EXTRA_EXP,
        SQLDATA_EXTRA_DMG
    };

public:
    /* constructor */
    Monster(uint32_t aUID, const Monster::Info& aInfo, Generator* aOwner = nullptr);

    /* destructor */
    virtual ~Monster();

public:
    /** Send the entity spawn msg. */
    virtual void sendShow(const Player& aPlayer) const;

    /** Called when the timer elapse. */
    virtual void timerElapsed() { /* TODO monster timerElapsed */  }

public:
    /** Get the minimum physical attack of the monster. */
    virtual int32_t getMinAtk() const { return mInfo.MinAtk; }
    /** Get the maximum physical attack of the monster. */
    virtual int32_t getMaxAtk() const { return mInfo.MaxAtk; }
    /** Get the physical defense of the monster. */
    virtual int32_t getDefense() const { return mInfo.Defense; }
    /** Get the magic attack of the monster. */
    virtual int32_t getMAtk() const { return mInfo.MinAtk; }
    /** Get the magic defense of the monster. */
    virtual int32_t getMDef() const { return mInfo.MDef; }

    /** Get the dexterity of the monster. */
    virtual uint8_t getDext() const { return mInfo.Dexterity; }
    /** Get the dodge of the monster. */
    virtual uint8_t getDodge() const { return mInfo.Dodge; }

public:
    /** Get the armet dropped by the monster. */
    uint8_t getDropArmet() { return mInfo.DropArmet; }
    /** Get the necklace dropped by the monster. */
    uint8_t getDropNecklace() { return mInfo.DropNecklace; }
    /** Get the armor dropped by the monster. */
    uint8_t getDropArmor() { return mInfo.DropArmor; }
    /** Get the ring dropped by the monster. */
    uint8_t getDropRing() { return mInfo.DropRing; }
    /** Get the weapon dropped by the monster. */
    uint8_t getDropWeapon() { return mInfo.DropWeapon; }
    /** Get the shield dropped by the monster. */
    uint8_t getDropShield() { return mInfo.DropShield; }
    /** Get the shoes dropped by the monster. */
    uint8_t getDropShoes() { return mInfo.DropShoes; }

private:
    const Info& mInfo; //!< the shared information of the monster
    Generator* mOwner; //!< the owner of the monster
};

#endif // _COPS_V7_EMULATOR_MONSTER_H
