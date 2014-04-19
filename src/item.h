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
class Player;
class Entity;
class QSqlQuery;

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
        uint8_t Ident;
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
        MAX_EQUIPMENT,

        POS_STORAGE = 201,
        POS_TRUNK = 202,
        POS_CHEST = 203,
        POS_GROUND = 254
    };

    enum Ident
    {
        IDENT_NONE = 0,
        IDENT_NOT_IDENT = 1,
        IDENT_CANNOT_REPAIR = 2,
        IDENT_NEVER_DAMAGE = 4
    };

    enum Sort
    {
        SORT_INVALID = -1,
        SORT_FINERY = 1,
        SORT_EXPEND = 10,
        SORT_ONE_HAND_WEAPON = 4,
        SORT_TWO_HANDS_WEAPON = 5,
        SORT_SHIELD = 9,
        SORT_OTHER = 7
    };

    enum Type
    {
        TYPE_INVALID = -1,
        TYPE_HELMET = 10000,
        TYPE_NECKLACE = 20000,
        TYPE_ARMOR = 30000,
        TYPE_RING = 50000,
        TYPE_SHOES = 60000,
        TYPE_MEDICINE_HP = 10000,
        TYPE_MEDICINE_MP = 11000,
        TYPE_SCROLL = 20000,
        TYPE_SCROLL_SPECIAL = 20000,
        TYPE_SCROLL_MSKILL = 21000,
        TYPE_SCROLL_SSKILL = 22000,
        TYPE_SCROLL_BSKILL = 23000,
        TYPE_SPECIAL_USE = 50000
    };

    static const uint8_t MONOPOLY_MASK                = 0x01;
    static const uint8_t STORAGE_MASK                 = 0x02;
    static const uint8_t DROP_HINT_MASK               = 0x04;
    static const uint8_t SELL_HINT_MASK               = 0x08;
    static const uint8_t NEVER_DROP_WHEN_DEAD_MASK    = 0x10;
    static const uint8_t SELL_DISABLE_MASK            = 0x20;

public:
    static const uint32_t CARD_FEE_POINT_1          = 780000;
    static const uint32_t CARD_FEE_POINT_2          = 780001;

public:
    /** Position of the SQL data in the result set. */
    enum SqlData_Info
    {
        SQLDATA_INFO_ID = 0,
        SQLDATA_INFO_NAME,
        SQLDATA_INFO_REQ_PROF,
        SQLDATA_INFO_REQ_WEAPONSKILL,
        SQLDATA_INFO_REQ_LEVEL,
        SQLDATA_INFO_REQ_SEX,
        SQLDATA_INFO_REQ_FORCE,
        SQLDATA_INFO_REQ_SPEED,
        SQLDATA_INFO_REQ_HEALTH,
        SQLDATA_INFO_REQ_SOUL,
        SQLDATA_INFO_MONOPOLY,
        SQLDATA_INFO_WEIGHT,
        SQLDATA_INFO_PRICE,
        SQLDATA_INFO_TASK,
        SQLDATA_INFO_MAX_ATK,
        SQLDATA_INFO_MIN_ATK,
        SQLDATA_INFO_DEFENSE,
        SQLDATA_INFO_DEXTERITY,
        SQLDATA_INFO_DODGE,
        SQLDATA_INFO_LIFE,
        SQLDATA_INFO_MANA,
        SQLDATA_INFO_AMOUNT,
        SQLDATA_INFO_AMOUNT_LIMIT,
        SQLDATA_INFO_IDENT,
        SQLDATA_INFO_GEM1,
        SQLDATA_INFO_GEM2,
        SQLDATA_INFO_MAGIC1,
        SQLDATA_INFO_MAGIC2,
        SQLDATA_INFO_MAGIC3,
        SQLDATA_INFO_MAGIC_ATK,
        SQLDATA_INFO_MAGIC_DEF,
        SQLDATA_INFO_ATK_RANGE,
        SQLDATA_INFO_ATK_SPEED
    };

    /** Position of the SQL data in the result set. */
    enum SqlData
    {
        SQLDATA_ID = 0,
        SQLDATA_TYPE,
        SQLDATA_OWNER_ID,
        SQLDATA_PLAYER_ID,
        SQLDATA_AMOUNT,
        SQLDATA_AMOUNT_LIMIT,
        SQLDATA_IDENT,
        SQLDATA_POSITION,
        SQLDATA_GEM1,
        SQLDATA_GEM2,
        SQLDATA_MAGIC1,
        SQLDATA_MAGIC2,
        SQLDATA_MAGIC3,
        SQLDATA_RESTRAIN,
        SQLDATA_BLESS,
        SQLDATA_ENCHANT,
        SQLDATA_SUSPICIOUS,
        SQLDATA_LOCKED,
        SQLDATA_COLOR
    };

public:
    static err_t createItem(Item** aOutItem, const QSqlQuery& aQuery);

public:
    static bool createItem(Item** aOutItem, const Item::Info& aInfo,
                           const Player& aPlayer);
    static bool createItem(Item** aOutItem, uint32_t aValue, Monster& aMonster, uint8_t aQuality = 0);

public:
    Item(uint32_t aUID, const Item::Info& aInfo);
    ~Item();

    err_t save() const;
    err_t erase() const;

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

    Entity* getOwner() const { return mOwner; }
    Player* getPlayer() const { return mPlayer; }

    uint16_t getAmount() const { return mAmount; }
    uint16_t getAmountLimit() const { return mAmountLimit; }

    Position getPosition() const { return mPosition; }
    uint8_t getIdent() const { return mIdent; }

    uint8_t getGem1() const { return mGem1; }
    uint8_t getGem2() const { return mGem2; }
    uint8_t getMagic1() const { return mMagic1; }
    uint8_t getMagic2() const { return mMagic2; }
    uint8_t getMagic3() const { return mMagic3; }
    uint8_t getBless() const { return mBless; }
    uint8_t getEnchant() const { return mEnchant; }
    uint32_t getRestrain() const { return mRestrain; }
    bool isSuspicious() const { return mSuspicious; }
    bool isLocked() const { return mLocked; }
    uint8_t getColor() const { return mColor; }

public:
    void setOwner(Entity* aOwner, bool aUpdate = true) { mOwner = aOwner; if (aUpdate) { save(); } }
    void setPlayer(Player* aPlayer, bool aUpdate = true) { mPlayer = aPlayer; if (aUpdate) { save(); } }

    void setAmount(uint16_t aAmount, bool aUpdate = false) { mAmount = aAmount; if (aUpdate) { save(); } }
    void setAmountLimit(uint16_t aAmountLimit, bool aUpdate = false) { mAmountLimit = aAmountLimit; if (aUpdate) { save(); } }

public:
    bool isNormal() const { return getItemSort() == SORT_EXPEND; }
    bool is1HWeapon() const { return getItemSort() == SORT_ONE_HAND_WEAPON; }
    bool is2HWeapon() const { return getItemSort() == SORT_TWO_HANDS_WEAPON; }
    bool isWeapon() const { return is1HWeapon() || is2HWeapon(); }
    bool isShield() const { return getItemSort() == SORT_SHIELD; }
    bool isOther() const { return getItemSort() == SORT_OTHER; }
    bool isFinery() const { return !isArrow() && getItemSort() == SORT_FINERY; }
    bool isArrow() const { return false; }
//    bool	IsArrow()		{ return CItem::IsArrow(GetInt(ITEMDATA_TYPE)); } //IsNormal() && GetItemType() == ITEMTYPE_ARROW; }
//    bool	IsDart()		{ return CItem::IsDart(GetInt(ITEMDATA_TYPE)); } //IsNormal() && GetItemType() == ITEMTYPE_DART; }
//    bool	IsSpell()		{ return CItem::IsSpell(GetInt(ITEMDATA_TYPE)); }
    bool isExpend() const { return isArrow() || (getItemSort() == SORT_EXPEND && getItemType() < TYPE_SPECIAL_USE); }
//    bool	IsHelmet()		{ return IsFinery() && GetItemType() == ITEMTYPE_HELMET; }
//    bool	IsNecklace()	{ return IsFinery() && GetItemType() == ITEMTYPE_NECKLACE; }
//    bool	IsArmor()		{ return IsFinery() && GetItemType() == ITEMTYPE_ARMOR; }
//    bool	IsRing()		{ return IsFinery() && GetItemType() == ITEMTYPE_RING; }
//    bool	IsBangle()		{ return IsFinery() && GetItemType() == ITEMTYPE_BANGLE; }	// ÊÇ·ñÊÖïí -- zlong 2004-02-03
//    bool	IsShoes()		{ return IsFinery() && GetItemType() == ITEMTYPE_SHOES; }
//    bool	IsEquipment()	{ return CItem::IsEquipment(GetInt(ITEMDATA_TYPE)); }
//    bool	IsBowSort()		{ return CItem::IsBowSort(GetInt(ITEMDATA_TYPE)); }
//    bool	IsBow()			{ return CItem::IsBow(GetInt(ITEMDATA_TYPE)); }
//    bool	IsCrossBow()	{ return CItem::IsCrossBow(GetInt(ITEMDATA_TYPE)); }
//    bool	IsActionItem()	{ return IsOther() && GetItemType() == ITEMTYPE_ACTIONITEM || IsSpell(); }
//    bool	IsTaskItem()	{ return IsOther() && GetItemType() == ITEMTYPE_TASKITEM; }
//    bool	IsGem()			{ return IsOther() && GetItemType() == ITEMTYPE_GEM; }
//    bool	IsNonsuch()		{ return GetQuality() == 9; }
    bool isGameCard() const { return mInfo.Id == CARD_FEE_POINT_1 || mInfo.Id == CARD_FEE_POINT_2; }
    //bool	IsMedicine()	{ return CItem::IsMedicine(GetInt(ITEMDATA_TYPE)); }
    bool isPickAxe() const { return 470 == ((mInfo.Id / 1000) % 1000); }

public:
    //    bool	IsHoldEnable()		{ return IsWeapon1() || IsWeapon2() || IsShield() || IsArrowSort(); }
    //	bool	IsEquipEnable()		{ return IsEquipment() || IsArrowSort() || IsSprite(); }
    //	bool	IsEatEnable()		{ return IsMedicine(); }
    //	bool	IsPileEnable()		{ return IsExpend() && GetInt(ITEMDATA_AMOUNTLIMIT) > 1; }
    bool isRepairEnable() const { return !isExpend() && !isNeedIdent() && mAmountLimit >= 100 && !isCannotRepair(); }
    bool isExchangeEnable() const { return ((mInfo.Monopoly & MONOPOLY_MASK) == 0); }
    bool isStorageEnable() const { return ((mInfo.Monopoly & STORAGE_MASK) == 0); }
    bool isSellEnable() const { return ((mInfo.Monopoly & SELL_DISABLE_MASK) == 0) && !isGameCard(); }
    bool isNeverDropWhenDead() const { return ((mInfo.Monopoly & NEVER_DROP_WHEN_DEAD_MASK) != 0); }
    //bool isNonsuchItem();
    //bool	IsNormalArrow()		{ return GetInt(ITEMDATA_TYPE) == NORMAL_ARROW_TYPE; }
    bool isDiscardable() const { return !isGameCard(); }
    bool isNeedIdent() const { return ((mInfo.Ident & (uint8_t)IDENT_NOT_IDENT) != 0); }
    bool isCannotRepair() const { return ((mInfo.Ident & (uint8_t)IDENT_CANNOT_REPAIR) != 0); }
    bool isNeverWear() const { return ((mInfo.Ident & (uint8_t)IDENT_NEVER_DAMAGE) != 0); }

public:
    uint32_t getSellPrice() const;
    uint32_t getRepairCost() const;

private:
    uint32_t getItemSort() const { return (mInfo.Id % 10000000) / 100000; }
    uint32_t getItemType() const { return (is1HWeapon() || is2HWeapon()) ? ((mInfo.Id % 100000) / 1000) * 1000 : ((mInfo.Id % 100000) / 10000) * 10000; }
    uint32_t getItemSubType() const { return (mInfo.Id % 1000000) / 1000; }

private:
    const uint32_t mUID;
    const Item::Info& mInfo;

    Entity* mOwner;
    Player* mPlayer;

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
    bool mSuspicious;
    bool mLocked;
    uint8_t mColor;
};

#endif // _COPS_V7_EMULATOR_ITEM_H_
