/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_PLAYER_H
#define _COPS_V7_EMULATOR_PLAYER_H

#include "common.h"
#include "advancedentity.h"
#include "client.h"
#include "item.h"
#include "timer.h"
#include "mstimer.h"
#include "msguserattrib.h"
#include <QMutex>
#include <string>
#include <deque>
#include <map>

class Magic;
class WeaponSkill;

/**
 * Base class of all players.
 */
class Player : public AdvancedEntity
{
    friend class Database; // the database can manipulate the Player data...

    PROHIBIT_COPY(Player); // constant UID, should be unique...

public:
    /** Sex of a male player. */
    static const uint8_t SEX_MALE               = 1;
    /** Sex of a female player. */
    static const uint8_t SEX_FEMALE             = 2;
    /** Sex of an asexual player. */
    static const uint8_t SEX_ASEXUAL            = 3;
    /** Sex containing both male and female. */
    static const uint8_t SEX_ALL                = 4;

    /** Look of a small male. */
    static const uint16_t LOOK_SMALL_MALE       = 1003;
    /** Look of a big male. */
    static const uint16_t LOOK_BIG_MALE         = 1004;
    /** Look of a small female. */
    static const uint16_t LOOK_SMALL_FEMALE     = 2001;
    /** Look of a big female. */
    static const uint16_t LOOK_BIG_FEMALE       = 2002;

    /** Default face of a male. */
    static const uint16_t FACE_MALE     = 67;
    /** Default face of a female. */
    static const uint16_t FACE_FEMALE   = 201;

    /** Default hair. */
    static const uint16_t HAIR_DEFAULT   = 310;

    /** The time interval between each addition of energy. */
    static const int ADD_ENERGY_SECS            = 5;
    /** The gained energy when standing. */
    static const uint8_t ADD_ENERGY_STAND       = 1;
    /** The gained energy when sit. */
    static const uint8_t ADD_ENERGY_SIT         = 7;

    /** Max size of the player's inventory. */
    static const size_t MAX_INVENTORY_SIZE = 40;

    /** List of all PK modes. */
    enum PkMode
    {
        /** Can attack any player and monster. */
        PKMODE_FREE = 0,
        /** Can only attack monsters. */
        PKMODE_SAFE = 1,
        /** Can attack any monster and enemies. */
        PKMODE_TEAM = 2,
        /** Can attack any monster and PKers. */
        PKMODE_ARRESTMENT = 3
    };

    /** Position of the SQL data in the result set. */
    enum SqlData
    {
        SQLDATA_ACCOUNT_ID = 0,
        SQLDATA_ID,
        SQLDATA_NAME,
        SQLDATA_MATE,
        SQLDATA_LOOKFACE,
        SQLDATA_HAIR,
        SQLDATA_MONEY,
        SQLDATA_MONEY_SAVED,
        SQLDATA_CONQUER_POINTS,
        SQLDATA_LEVEL,
        SQLDATA_EXP,
        SQLDATA_FORCE,
        SQLDATA_DEXTERITY,
        SQLDATA_HEALTH,
        SQLDATA_SOUL,
        SQLDATA_ADD_POINTS,
        SQLDATA_LIFE,
        SQLDATA_MANA,
        SQLDATA_PROFESSION,
        SQLDATA_PK_POINTS,
        SQLDATA_VIRTUE,
        SQLDATA_NOBILITY,
        SQLDATA_METEMPSYCHOSIS,
        SQLDATA_SYNDICATE_ID,
        SQLDATA_RECORD_MAP,
        SQLDATA_RECORD_X,
        SQLDATA_RECORD_Y,
        SQLDATA_LAST_LOGIN
    };

public:
    /* constructor */
    Player(Client& aClient, uint32_t aUID);

    /* destructor */
    virtual ~Player();

public:
    /**
     * Enter the map specified by the Id.
     * It will send all the info and update the broadcast set.
     */
    void enterMap();

    /**
     * Leave the map specified by the Id.
     *It will send all the info and clear the broadcast set.
     */
    void leaveMap();

    /**
     * Try to move the player to the specified map and position.
     *
     * @param[in]   aMapId     the new map Id
     * @param[in]   aX         the new X coord
     * @param[in]   aY         the new Y coord
     *
     * @retval TRUE on success
     * @returns FALSE otherwise
     */
    bool move(uint32_t aMapId, uint16_t aX, uint16_t aY);

    /**
     * Try to move the player to the specified position on the current map.
     *
     * @param[in]   aX         the new X coord
     * @param[in]   aY         the new Y coord
     * @param[in]   aDir       the new direction
     *
     * @retval TRUE on success
     * @returns FALSE otherwise
     */
    bool move(uint16_t aX, uint16_t aY, uint8_t aDir);

    /**
     * Kick back the player to the previous position.
     */
    void kickBack();

    /** Send the entity spawn msg. */
    virtual void sendShow(const Player& aPlayer) const;

    /** Called when the timer elapse. */
    virtual void timerElapsed();

    /**
     * Distribute the points.
     *
     * @param[in]   aForce         the additional force
     * @param[in]   aHealth        the additional health
     * @param[in]   aDexterity     the additional dexterity
     * @param[in]   aSoul          the additional soul
     */
    void allot(uint8_t aForce, uint8_t aHealth, uint8_t aDexterity, uint8_t aSoul);

    /**
     * Send the items information.
     */
    void sendItemSet() const;

    /**
     * Send the weapon skills information.
     */
    void sendWeaponSkillSet() const;

    /**
     * Send the magic skills information.
     */
    void sendMagicSkillSet() const;

    /**
     * Save all the items.
     */
    void saveAllItem() const;

public:
    /** Send a system message. */
    void sendSysMsg(const char* aFmt, ...) const;

    /**
     * Process the MsgTick data.
     *
     * @param[in]    aClientTime      the client time
     * @param[in]    aMsgCount        the client's message count
     */
    void processTick(int32_t aClientTime, uint32_t aMsgCount);

    /**
     * Send a message to the client.
     * The TQ seal will be appended at the end.
     *
     * @param[in]  aMsg     the message to send
     */
    inline void send(const Msg* aMsg) const { mClient.send(aMsg); }

    /**
     * Disconnect the client from the server.
     */
    inline void disconnect() { mClient.disconnect(); }

public:
    /** Get the name of the mate of the player. */
    const char* getMate() const { return mMate.c_str(); }

    /** Get the sex of the player. */
    uint8_t getSex() const { return (mLook / 1000) % 10; }

    /** Get the hair ID of the player. */
    uint16_t getHair() const { return mHair; }

    /** Get the profession of the player. */
    uint8_t getProfession() const { return mProfession; }
    /** Get the metempsychosis (reborn) of the player. */
    uint8_t getMetempsychosis() const { return mMetempsychosis; }
    /** Get the experience points of the player. */
    uint64_t getExp() const { return mExp; }

    /** Get the force of the player. */
    uint16_t getForce() const { return mForce; }
    /** Get the dexterity of the player. */
    uint16_t getDexterity() const { return mDexterity; }
    /** Get the health of the player. */
    uint16_t getHealth() const { return mHealth; }
    /** Get the soul of the player. */
    uint16_t getSoul() const { return mSoul; }
    /** Get the additional points of the player. */
    uint16_t getAddPoints() const { return mAddPoints; }

    /** Get the money of the player. */
    uint32_t getMoney() const { return mMoney; }
    /** Get the CPs of the player. */
    uint32_t getCPs() const { return mCPs; }
    /** Get the Pk points of the player. */
    int16_t getPkPoints() const { return mPkPoints; }
    /** Get the virtue points of the player. */
    int32_t getVirtue() const { return mVirtue; }

    /** Get the energy of the player. */
    uint8_t getEnergy() const { return mEnergy; }
    /** Get the XP of the player. */
    uint8_t getXP() const { return mXP; }

    /** Get the previous map Id of the player. */
    uint32_t getPrevMap() const { return mPrevMap; }
    /** Get the previous X position of the player. */
    uint16_t getPrevX() const { return mPrevX; }
    /** Get the previous Y position of the player. */
    uint16_t getPrevY() const { return mPrevY; }

    /** Get the PK mode of the player. */
    PkMode getPkMode() const { return mPkMode; }

    /** Set the PK mode of the player. */
    void setPkMode(PkMode aPkMode) { mPkMode = aPkMode; }

public:
    /** Get the minimum physical attack of the player. */
    virtual int32_t getMinAtk() const;
    /** Get the maximum physical attack of the player. */
    virtual int32_t getMaxAtk() const;
    /** Get the physical defense of the player. */
    virtual int32_t getDefense() const;
    /** Get the magic attack of the player. */
    virtual int32_t getMAtk() const;
    /** Get the magic defense of the player. */
    virtual int32_t getMDef() const;

    /** Get the additional physical attack of the player. */
    int32_t getAdditionAtk() const;
    /** Get the additional physical defense of the player. */
    int32_t getAdditionDef() const;
    /** Get the additional magical attack of the player. */
    int32_t getAdditionMAtk() const;
    /** Get the additional magical defense of the player. */
    int32_t getAdditionMDef() const;

    /** Get the dexterity of the player. */
    virtual uint8_t getDext() const;
    /** Get the dodge of the player. */
    virtual uint8_t getDodge() const { return 0; /* TODO getDodge() */ }

public:
    /** Get the maximum hit points of the player. */
    virtual uint16_t getMaxHP() const;

    /** Get the current mana points of the player. */
    uint16_t getCurMP() const { return mCurMP; }
    /** Get the maximum mana points of the player. */
    uint16_t getMaxMP() const;

    /** Get the maximum XP points of the player. */
    uint8_t getMaxXP() const;
    /** Get the maximum energy of the player. */
    uint8_t getMaxEnergy() const;

    /** Determine whether or not the player is transformed in ghost. */
    bool isGhost() const { return false; } // TODO implement ghost

public:
    /** Get the equipment by its position. */
    Item* getEquipByPos(uint8_t aPos) const
    { ASSERT(aPos < Item::MAX_EQUIPMENT); return mEquipment[aPos]; }

    /** Get the item by its unique ID. */
    Item* getItem(uint32_t aUID) const;

    /**
     * Determine whether or not an item can be equipped.
     *
     * @param[in]   aItem      the item to equip
     * @param[in]   aPosition  the target position of the item
     *
     * @retval TRUE if the item can be equipped
     * @return FALSE otherwise
     */
    bool canEquipItem(const Item& aItem, Item::Position aPosition) const;

    /**
     * Use an item. It may equip it, launch the task or eat it.
     *
     * @param[in]   aItem      the item to use, equip or eat
     * @param[in]   aPosition  the target position of the item (when equipping)
     * @param[in]   aSend      whether or not the client must be updated
     *
     * @retval TRUE on success
     * @return FALSE otherwise
     */
    bool useItem(Item& aItem, Item::Position aPosition, bool aSend);

    /**
     * Equip an item.
     *
     * @param[in]   aItem      the item to equip
     * @param[in]   aPosition  the target position of the item
     * @param[in]   aSend      whether or not the client must be updated
     *
     * @retval TRUE on success
     * @return FALSE otherwise
     */
    bool equipItem(Item& aItem, Item::Position aPosition, bool aSend);

    /**
     * Unequip an item without signaling the client.
     *
     * @param[in]   aPosition  the position of the item to unequip
     */
    void unequipOnly(Item::Position aPosition);

    /**
     * Award an item to the player.
     *
     * @param[in]   aInfo    the info of the item to award
     * @param[in]   aSend    whether or not the client must be updated
     *
     * @retval TRUE on success
     * @return FALSE otherwise
     */
    bool awardItem(const Item::Info& aInfo, bool aSend);

    /**
     * Add an item to the player's inventory.
     *
     * @param[in]   aItem    the item to add
     * @param[in]   aSend    whether or not the client must be updated
     *
     * @retval TRUE on success
     * @return FALSE otherwise
     */
    bool addItem(Item* aItem, bool aSend);

    /** Erase the item from the inventory and the database. */
    bool eraseItem(uint32_t aUID, bool aSend);

public:
    /** Get the tick of the latest cool effect */
    time_t getLastCoolShow() const { return mLastCoolShow; }

    /** Set the tick of the latest cool effect */
    void setLastCoolShow(time_t aTick) { mLastCoolShow = aTick; }

public:
    /**
     * Add money to the player.
     *
     * @param[in]   aMoney   the money to add
     * @param[in]   aSend    whether or not the client must be updated
     *
     * @retval TRUE on success
     * @return FALSE otherwise
     */
    bool gainMoney(uint32_t aMoney, bool aSend);

    /**
     * Add CPs to the player.
     *
     * @param[in]   aCPs     the CPs to add
     * @param[in]   aSend    whether or not the client must be updated
     *
     * @retval TRUE on success
     * @return FALSE otherwise
     */
    bool gainCPs(uint32_t aCPs, bool aSend);

    /**
     * Remove money to the player.
     *
     * @param[in]   aMoney   the money to remove
     * @param[in]   aSend    whether or not the client must be updated
     *
     * @retval TRUE on success
     * @return FALSE otherwise
     */
    bool spendMoney(uint32_t aMoney, bool aSend);

    /**
     * Remove CPs to the player.
     *
     * @param[in]   aCPs     the CPs to remove
     * @param[in]   aSend    whether or not the client must be updated
     *
     * @retval TRUE on success
     * @return FALSE otherwise
     */
    bool spendCPs(uint32_t aCPs, bool aSend);

    /**
     * Add/remove a value from the player's attribute.
     *
     * @param[in]   aType       the type of the attribute
     * @param[in]   aData       the value to add/remove
     * @param[in]   aSend       whether or not the client must be updated
     * @param[in]   aBroadcast  whether or not the broadcast set must be updated
     *
     * @retval TRUE on success
     * @return FALSE otherwise
     */
    bool addAttrib(MsgUserAttrib::UserAttrType aType, int64_t aData,
                   bool aSend, bool aBroadcast);

private:
    /** Delete all the items of the player. */
    void deleteAllItem();

    /** Delete all the weapon skills of the player. */
    void deleteAllWeaponSkill();

    /** Delete all the magic skills of the player. */
    void deleteAllMagicSkill();

private:
    Client& mClient; //!< the client

    std::string mMate; //!< the name of the mate

    uint16_t mHair; //!< the hair of the player

    uint8_t mProfession; //!< the profession of the player
    uint8_t mMetempsychosis; //!< the metempsychosis of the player
    uint64_t mExp; //!< the exp of the player

    uint16_t mForce; //!< the force of the player
    uint16_t mDexterity; //!< the dexterity of the player
    uint16_t mHealth; //!< the health of the player
    uint16_t mSoul; //!< the soul of the player
    uint16_t mAddPoints; //!< the additional points of the player

    uint16_t mCurMP; //!< the current mana points of the player

    uint32_t mMoney; //!< the money of the player
    uint32_t mCPs; //!< the Conquer points of the player
    int16_t mPkPoints; //!< the Pk points of the player
    int32_t mVirtue; //!< the virtue points of the player

    uint8_t mEnergy; //!< the energy of the player
    uint8_t mXP; //!< the XP of the player

    uint32_t mPrevMap; //!< the previous map Id
    uint16_t mPrevX; //!< the previous X coord
    uint16_t mPrevY; //!< the previous Y coord

    PkMode mPkMode; //!< the Pk mode of the player

    Item* mEquipment[Item::MAX_EQUIPMENT]; //!< the equipment of the player
    std::map<uint32_t, Item*> mInventory; //!< the inventory of the player
    mutable QMutex mInventoryMutex; //!< the mutex to access the inventory

    std::map<uint16_t, WeaponSkill*> mWeaponSkills; //!< the weapon skills of the player
    mutable QMutex mWeaponSkillsMutex; //!< the mutex to access the weapon skills

    std::map<uint16_t, Magic*> mMagics; //!< the magic skills of the player
    mutable QMutex mMagicsMutex; //!< the mutex to access the magic skills

    // MsgTick protection
    uint32_t mMsgCount; //!< the last msg count
    int32_t mFirstClientTick; //!< the tick of first client response
    int32_t mLastClientTick; //!< the tick of latest client response
    int32_t mLastRcvClientTick; //!< the latest receive of the client tick
    int32_t mFirstServerTick; //!< the tick of first server request
    int32_t mLastServerTick; //!< the tick of the latest server request
    std::deque<int32_t> mServerTicks; //!< the server ticks

    time_t mLastCoolShow; //!< the tick of the latest cool effect
    Timer mLastAddEnergy; //!< the timer of the latest "add" ernergy
};

#endif // _COPS_V7_EMULATOR_PLAYER_H
