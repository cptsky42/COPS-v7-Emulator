/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_ITEM_H_
#define _COPS_V7_EMULATOR_ITEM_H_

#include "common.h"
#include <string>

class Monster;

class Item
{
public:
    struct Info
    {
    public:
        uint32_t Id;
        std::string Name;
        uint8_t ReqProf;
        uint8_t ReqWeaponSkill;
        uint8_t ReqLevel;
        uint8_t ReqSex;
        uint16_t ReqForce;
        uint16_t ReqSpeed;
        uint16_t ReqHealth;
        uint16_t ReqSoul;
        uint8_t Monopoly;
        uint16_t Weight;
        uint32_t Price;
        uint32_t Task; //TODO: ItemTask*
        uint16_t MaxAtk;
        uint16_t MinAtk;
        int16_t Defense;
        int16_t Dexterity;
        int16_t Dodge;
        int16_t Life;
        int16_t Mana;
        uint16_t Amount;
        uint16_t AmountLimit;
        uint8_t Status;
        uint8_t Gem1;
        uint8_t Gem2;
        uint8_t Magic1;
        uint8_t Magic2;
        uint8_t Magic3;
        uint16_t MagicAtk;
        uint16_t MagicDef;
        uint16_t AtkRange;
        uint16_t AtkSpeed;
    };

    enum Position
    {
        POS_INVENTORY = 0,
        POS_ARMET = 1,
        POS_NECKLACE = 2,
        POS_ARMOR = 3,
        POS_RWEAPON = 4,
        POS_LWEAPON = 5,
        POS_RING = 6,
        POS_BOTTLE = 7,
        POS_SHOES = 8,
        POS_GARMENT = 9,
        MAX_EQUIPMENT
    };

    /** Position of the SQL data in the result set. */
    enum SqlData
    {
        SQLDATA_ID = 0,
        SQLDATA_NAME,
        SQLDATA_REQ_PROF,
        SQLDATA_REQ_WEAPONSKILL,
        SQLDATA_REQ_LEVEL,
        SQLDATA_REQ_SEX,
        SQLDATA_REQ_FORCE,
        SQLDATA_REQ_SPEED,
        SQLDATA_REQ_HEALTH,
        SQLDATA_REQ_SOUL,
        SQLDATA_MONOPOLY,
        SQLDATA_WEIGHT,
        SQLDATA_PRICE,
        SQLDATA_TASK,
        SQLDATA_MAX_ATK,
        SQLDATA_MIN_ATK,
        SQLDATA_DEFENSE,
        SQLDATA_DEXTERITY,
        SQLDATA_DODGE,
        SQLDATA_LIFE,
        SQLDATA_MANA,
        SQLDATA_AMOUNT,
        SQLDATA_AMOUNT_LIMIT,
        SQLDATA_STATUS,
        SQLDATA_GEM1,
        SQLDATA_GEM2,
        SQLDATA_MAGIC1,
        SQLDATA_MAGIC2,
        SQLDATA_MAGIC3,
        SQLDATA_MAGIC_ATK,
        SQLDATA_MAGIC_DEF,
        SQLDATA_ATK_RANGE,
        SQLDATA_ATK_SPEED
    };

public:
    static bool createItem(Item** aOutItem, uint32_t aValue, Monster& aMonster, uint8_t aQuality = 0);

public:
    Item(uint32_t aUID, const Item::Info& aInfo);
    ~Item();

public:
    uint32_t getType() const { return mInfo.Id; }
    const char* getName() const { return mInfo.Name.c_str(); }
    uint8_t getReqProf() const { return mInfo.ReqProf; }
    uint8_t getReqWeaponSkill() const { return mInfo.ReqWeaponSkill; }
    uint8_t getReqLevel() const { return mInfo.ReqLevel; }
    uint8_t getReqSex() const { return mInfo.ReqSex; }
    uint16_t getReqForce() const { return mInfo.ReqForce; }
    uint16_t getReqSpeed() const { return mInfo.ReqSpeed; }
    uint16_t getReqHealth() const { return mInfo.ReqHealth; }
    uint16_t getReqSoul() const { return mInfo.ReqSoul; }
    uint8_t getMonopoly() const { return mInfo.Monopoly; }
    uint16_t getWeight() const { return mInfo.Weight; }
    uint32_t getPrice() const { return mInfo.Price; }
    uint16_t getMaxAtk() const { return mInfo.MaxAtk; }
    uint16_t getMinAtk() const { return mInfo.MinAtk; }
    int16_t getDefense() const { return mInfo.Defense; }
    int16_t getDexterity() const { return mInfo.Dexterity; }
    int16_t getDodge() const { return mInfo.Dodge; }
    int16_t getLife() const { return mInfo.Life; }
    int16_t getMana() const { return mInfo.Mana; }
    uint16_t getMagicAtk() const { return mInfo.MagicAtk; }
    uint16_t getMagicDef() const { return mInfo.MagicDef; }
    uint16_t getAtkRange() const { return mInfo.AtkRange; }
    uint16_t getAtkSpeed() const { return mInfo.AtkSpeed; }

public:
    uint32_t getUID() const { return mUID; }

    uint16_t getAmount() const { return mAmount; }
    uint16_t getAmountLimit() const { return mAmountLimit; }

    uint8_t getIdent() const { return mIdent; }
    Position getPosition() const { return mPosition; }

    uint8_t getGem1() const { return mGem1; }
    uint8_t getGem2() const { return mGem2; }
    uint8_t getMagic1() const { return mMagic1; }
    uint8_t getMagic2() const { return mMagic2; }
    uint8_t getMagic3() const { return mMagic3; }
    uint8_t getBless() const { return mBless; }
    uint8_t getEnchant() const { return mEnchant; }
    uint32_t getRestrain() const { return mRestrain; }
    bool isLocked() const { return mLocked; }
    bool isSuspicious() const { return mSuspicious; }
    uint8_t getColor() const { return mColor; }

private:
    const uint32_t mUID;
    const Item::Info& mInfo;

    uint16_t mAmount;
    uint16_t mAmountLimit;
    uint8_t mIdent;
    Position mPosition;

    uint8_t mGem1;
    uint8_t mGem2;
    uint8_t mMagic1; // Attr
    uint8_t mMagic2; // ???
    uint8_t mMagic3; // Plus
    uint8_t mBless;
    uint8_t mEnchant;
    uint32_t mRestrain;
    bool mLocked;
    bool mSuspicious;
    uint8_t mColor;
};

#endif // _COPS_V7_EMULATOR_ITEM_H_
