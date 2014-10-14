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
class ItemTask;
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
        ItemTask* Task;
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
        uint8_t FrayMode;
        uint8_t RepairMode;
        uint8_t TypeMask;
        uint32_t CPs;

    public:
        /* constructor */
        Info()
            : Id(0), Name("None"),
              ReqProf(0), ReqWeaponSkill(0), ReqLevel(0), ReqSex(0),
              ReqForce(0), ReqSpeed(0), ReqHealth(0), ReqSoul(0),
              Monopoly(0), Weight(0), Price(0), Task(nullptr),
              MaxAtk(0), MinAtk(0), Defense(0), Dexterity(0), Dodge(0), Life(0), Mana(0),
              Amount(0), AmountLimit(0), Ident(0), Gem1(0), Gem2(0), Magic1(0), Magic2(0), Magic3(0),
              MagicAtk(0), MagicDef(0), AtkRange(0), AtkSpeed(0),
              FrayMode(0), RepairMode(0), TypeMask(0), CPs(0)
        {

        }
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
        SORT_ONE_HAND_WEAPON = 4,
        SORT_TWO_HANDS_WEAPON = 5,
        SORT_SHIELD = 9,
        SORT_OTHER = 7,
        SORT_EXPEND = 10
    };

    enum Type
    {
        TYPE_INVALID = -1,
        TYPE_HELMET = 10000,
        TYPE_NECKLACE = 20000,
        TYPE_ARMOR = 30000,
        TYPE_RING = 50000,
        TYPE_SHOES = 60000,
        TYPE_SCROLL = 20000,
        TYPE_SPECIAL_USE = 50000,
        TYPE_GEM = 00000,
        TYPE_MEDICINE = 00000,
        TYPE_TASKITEM = 10000,
        TYPE_ACTIONITEM = 20000,
        TYPE_GAMECARD = 80000
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
        SQLDATA_INFO_ATK_SPEED,
        SQLDATA_INFO_FRAY_MODE,
        SQLDATA_INFO_REPAIR_MODE,
        SQLDATA_INFO_TYPE_MASK,
        SQLDATA_INFO_CPS
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
    /**
     * Create an item object from a SQL query result set.
     *
     * @param[out]  aOutItem  a pointer that will receive the object
     * @param[in]   aQuery    the SQL result set
     *
     * @retval ERROR_SUCCESS on success
     * @returns An error code otherwise
     */
    static err_t createItem(Item** aOutItem, const QSqlQuery& aQuery);

public:
    static bool createItem(Item** aOutItem, const Item::Info& aInfo,
                           const Player& aPlayer);
    static bool createItem(Item** aOutItem, uint32_t aValue, Monster& aMonster, uint8_t aQuality = 0);

public:
    Item(uint32_t aUID, const Item::Info& aInfo);

    /* destructor */
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
    ItemTask* getTask() const { return mInfo.Task; }
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
    /**
     * Set the owner of the item.
     *
     * @param[in]   aOwner    the new owner of the item
     * @param[in]   aUpdate   (optional) determine whether or not the database must be updated
     *                        The default value is true.
     */
    void setOwner(Entity* aOwner, bool aUpdate = true) { mOwner = aOwner; if (aUpdate) { save(); } }

    /**
     * Set the owner (player) of the item.
     *
     * @param[in]   aPlayer   the new owner (player) of the item
     * @param[in]   aUpdate   (optional) determine whether or not the database must be updated
     *                        The default value is true.
     */
    void setPlayer(Player* aPlayer, bool aUpdate = true) { mPlayer = aPlayer; if (aUpdate) { save(); } }

    /**
     * Set the position of the item.
     *
     * @param[in]   aPosition the new position of the item
     * @param[in]   aUpdate   (optional) determine whether or not the database must be updated
     *                        The default value is true.
     */
    void setPosition(Item::Position aPosition, bool aUpdate = true) { mPosition = aPosition; if (aUpdate) { save(); } }

    /**
     * Set the amount of the item.
     *
     * @param[in]   aAmount   the new amount of the item
     * @param[in]   aUpdate   (optional) determine whether or not the database must be updated
     *                        The default value is false.
     */
    void setAmount(uint16_t aAmount, bool aUpdate = false) { mAmount = aAmount; if (aUpdate) { save(); } }

    /**
     * Set the amount limit of the item.
     *
     * @param[in] aAmountLimit  the new amount limit of the item
     * @param[in] aUpdate       (optional) determine whether or not the database must be updated
     *                          The default value is false.
     */
    void setAmountLimit(uint16_t aAmountLimit, bool aUpdate = false) { mAmountLimit = aAmountLimit; if (aUpdate) { save(); } }

public:
    /** Determine whether or not the item is a one-handed weapon. */
    bool is1HWeapon() const { return getItemSort() == SORT_ONE_HAND_WEAPON; }
    /** Determine whether or not the item is a two-handed weapon. */
    bool is2HWeapon() const { return getItemSort() == SORT_TWO_HANDS_WEAPON; }
    /** Determine whether or not the item is a weapon. */
    bool isWeapon() const { return is1HWeapon() || is2HWeapon(); }
    /** Determine whether or not the item is a shield. */
    bool isShield() const { return getItemSort() == SORT_SHIELD; }
    /** Determine whether or not the item is an arrow. */
    bool isArrow() const { return false; } // { return CItem::IsArrow(GetInt(ITEMDATA_TYPE)); } //IsNormal() && GetItemType() == ITEMTYPE_ARROW; }
    /** Determine whether or not the item is finery (helmet, armor, rign, necklace, etc) */
    bool isFinery() const { return !isArrow() && getItemSort() == SORT_FINERY; }
    /** Determine whether or not the item is an helmet. */
    bool isHelmet() const { return isFinery() && getItemType() == TYPE_HELMET; }
    /** Determine whether or not the item is a necklace. */
    bool isNecklace() const { return isFinery() && getItemType() == TYPE_NECKLACE; }
    /** Determine whether or not the item is an armor. */
    bool isArmor() const { return isFinery() && getItemType() == TYPE_ARMOR; }
    /** Determine whether or not the item is a ring. */
    bool isRing() const { return isFinery() && getItemType() == TYPE_RING; }
    /** Determine whether or not the item is a pair of shoes. */
    bool isShoes() const { return isFinery() && getItemType() == TYPE_SHOES; }

    bool isOther() const { return getItemSort() == SORT_OTHER; }
    bool isSpell() const { return isExpend() && getItemType() == TYPE_SCROLL; }
    bool isExpend() const { return isArrow() || (getItemSort() == SORT_EXPEND && getItemType() < TYPE_SPECIAL_USE); }
    bool isEquipment() const { return (!isArrow() && (getItemSort() >= SORT_FINERY && getItemSort() <= SORT_TWO_HANDS_WEAPON)) || isShield(); }
    bool isBow() const { return getItemSort() == SORT_TWO_HANDS_WEAPON && getItemType() == 500; /* TODO TYPE==BOW */ }
    bool isActionItem() const { return (isOther() && getItemType() == TYPE_ACTIONITEM) || isSpell(); }
    bool isTaskItem() const { return isOther() && getItemType() == TYPE_TASKITEM; }
    bool isGem() const { return isOther() && getItemType() == TYPE_GEM; }
    bool isGameCard() const { return mInfo.Id == CARD_FEE_POINT_1 || mInfo.Id == CARD_FEE_POINT_2; }
    bool isMedicine() const { return isExpend() && (getItemType() == TYPE_MEDICINE && (getItemSubType() <= 2)); }
    bool isPickAxe() const { return 470 == ((mInfo.Id / 1000) % 1000); }

    // Type = 10000, Subtype = 710, Sort = 7 => Quest item ? Not usable ?
    // Type = 20000, Subtype = 725, Sort = 7 => Skills
    // Type = 30000, Subtype = 730, Sort = 7 => Stones
    // Type = 50000, Subtype = 750, Sort = 7 => CloudSaint's jar
    // Type = 80000, Subtype = 780, Sort = 7 => Game Card
    // Type = 90000, Subtype = 790, Sort = 7 => Dis City Item ?

    // Type = 0, Sort = 10 Subtype = 0 & 1 => HP, 2 = MP

    // Type = 50000, Subtype = 50, Sort = 10 => Arrow
    // Type = 60000, Subtype = 60, Sort = 10 => Scroll / WindSpell
    // Type = 72000, Subtype = 72, Sort = 10 => Mine / Ore Gold etc
    // Type = 80000, Subtype = 80, Sort = 10 => Emerald
    // Type = 88000, Subtype = 88, Sort = 10 => DB, Meteor
    // Type = 90000  | 91000, Subtype, Sort = 10 => Money
    // Type = 0, Sybtype = 0, Sort = 11 => Sash
    // Type = 0, Subtype = 0, Sort = 12 => PrayingStones, Drill
    // Type = 0, Subtype = 0, Sort = 21 => Trophies / Gourd

    //    bool	IsNonsuch()		{ return GetQuality() == 9; }

public:
    /** Determine whether or not the item can be held. */
    bool isHoldEnable() const { return is1HWeapon() || is2HWeapon() || isShield() || isArrow(); }
    /** Determine whether or not the item can be equipped. */
    bool isEquipEnable() const { return isEquipment() || isArrow(); }
    /** Determine whether or not the item can be ate. */
    bool isEatEnable() const { return isMedicine(); }
    /** Determine whether or not the item can be stacked. */
    bool isPileEnable() const { return isExpend() && mInfo.AmountLimit > 1; }

    /** Determine whether or not the item can be repaired. */
    bool isRepairEnable() const { return !isExpend() && !isNeedIdent() && mAmountLimit >= 100 && !((mInfo.Ident & (uint8_t)IDENT_CANNOT_REPAIR) != 0); }
    /** Determine whether or not the item can be exchanged. */
    bool isExchangeEnable() const { return ((mInfo.Monopoly & MONOPOLY_MASK) == 0); }
    /** Determine whether or not the item can be stored. */
    bool isStorageEnable() const { return ((mInfo.Monopoly & STORAGE_MASK) == 0); }
    /** Determine whether or not the item can be sold. */
    bool isSellEnable() const { return ((mInfo.Monopoly & SELL_DISABLE_MASK) == 0) && !isGameCard(); }
    /** Determine whether or not the item can be dropped on death. */
    bool isNeverDropWhenDead() const { return ((mInfo.Monopoly & NEVER_DROP_WHEN_DEAD_MASK) != 0); }
    /** Determine whether or not the item can be discarded. */
    bool isDiscardable() const { return !isGameCard(); }
    /** Determine whether or not the item need ident. */
    bool isNeedIdent() const { return ((mInfo.Ident & (uint8_t)IDENT_NOT_IDENT) != 0); }
    /** Determine whether or not the item never wear. */
    bool isNeverWear() const { return ((mInfo.Ident & (uint8_t)IDENT_NEVER_DAMAGE) != 0); }

    //bool isNonsuchItem();

public:
    /** Get the sell price of the item. */
    uint32_t getSellPrice() const;
    /** Get the repair cost for the item. */
    uint32_t getRepairCost() const;

public:
    /** Get the sort of the item. @warning Unrecommended function. */
    inline uint32_t getItemSort() const { return (mInfo.Id % 10000000) / 100000; }
    /** Get the type of the item. @warning Unrecommended function. */
    inline uint32_t getItemType() const { return (is1HWeapon() || is2HWeapon()) ? ((mInfo.Id % 100000) / 1000) * 1000 : ((mInfo.Id % 100000) / 10000) * 10000; }
    /** Get the subtype of the item. @warning Unrecommended function. */
    inline uint32_t getItemSubType() const { return (mInfo.Id % 1000000) / 1000; }

private:
    const uint32_t mUID; //!< unique ID of the item
    const Item::Info& mInfo; //!< generic info of the item

    Entity* mOwner; //!< entity owning the item
    Player* mPlayer; //!< player owning the item

    uint16_t mAmount; //!< amount / durability of the item
    uint16_t mAmountLimit; //!< amount limit of the item
    uint8_t mIdent; //!< ident of the item
    Position mPosition; //!< position of the item

    uint8_t mGem1; //!< first gem of the item
    uint8_t mGem2; //!< second gem of the item
    uint8_t mMagic1; //!< attribute of the item (e.g. poison, HP, MP)
    uint8_t mMagic2; //!< unknown attribute of the item, probably unused
    uint8_t mMagic3; //!< crafting of the item (plus)
    uint8_t mBless; //!< blessing of the item
    uint8_t mEnchant; //!< enchant of the item
    uint32_t mRestrain; //!< restrain of the item (see restrain.ini), unused on English servers
    bool mSuspicious; //!< whether the item is suspicious or not
    bool mLocked; //!< whether the item is locked or not
    uint8_t mColor; //!< color of the item
};

#endif // _COPS_V7_EMULATOR_ITEM_H_
