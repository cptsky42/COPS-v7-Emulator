/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_ENTITY_H
#define _COPS_V7_EMULATOR_ENTITY_H

#include "common.h"
#include <time.h>
#include <map>
#include <QMutex>

class Player;
class Msg;

/**
 * Base class of all entity on a map.
 */
class Entity
{
    PROHIBIT_COPY(Entity); // constant UID, should be unique...

public:
    /** First valid UID for a NPC. */
    static const uint32_t SCENEID_FIRST     = 1;
    /** First valid UID for a system NPC. */
    static const uint32_t SYSNPCID_FIRST    = 1;
    /** Last valid UID for a system NPC. */
    static const uint32_t SYSNPCID_LAST     = 99999;
    /** First valid UID for a user/dynamic NPC. */
    static const uint32_t DYNANPCID_FIRST   = 100001;
    /** Last valid UID for a user/dynamic NPC. */
    static const uint32_t DYNANPCID_LAST    = 199999;
    /** Last valid UID for a NPC. */
    static const uint32_t SCENEID_LAST      = 299999;

    /** First valid UID for an advanced NPC. */
    static const uint32_t NPCSERVERID_FIRST = 400001;
    /** First valid UID for a monster. */
    static const uint32_t MONSTERID_FIRST   = 400001;
    /** Last valid UID for a monster. */
    static const uint32_t MONSTERID_LAST    = 499999;
    /** First valid UID for a pet. */
    static const uint32_t PETID_FIRST       = 500001;
    /** Last valid UID for a pet. */
    static const uint32_t PETID_LAST        = 599999;
    /** Last valid UID for an advanced NPC. */
    static const uint32_t NPCSERVERID_LAST  = 699999;

    /** First valid UID for a called pet. */
    static const uint32_t CALLPETID_FIRST   = 700001;
    /** Last valid UID for a called pet. */
    static const uint32_t CALLPETID_LAST    = 799999;

    // unused for the moment ?
    static const uint32_t TRAPID_FIRST      = 900001;
    static const uint32_t MAGICTRAPID_FIRST = 900001;
    static const uint32_t MAGICTRAPID_LAST  = 989999;
    static const uint32_t SYSTRAPID_FIRST   = 990001;
    static const uint32_t SYSTRAPID_LAST    = 999999;
    static const uint32_t TRAPID_LAST       = 999999;

    /** First valid UID for a player. */
    static const uint32_t PLAYERID_FIRST    = 1000000;
    /** Last valid UID for a player. */
    static const uint32_t PLAYERID_LAST     = 1999999999;

public:
    /** Number of cells per block. */
    static const int32_t CELLS_PER_BLOCK = 18;
    /** Number of cells per view. */
    static const int32_t CELLS_PER_VIEW = 18;

public:
    /** Determine wheter or not the entity linked to the UID is a NPC. */
    static bool isNpc(uint32_t aUID) { return aUID >= SCENEID_FIRST && aUID <= SCENEID_LAST; }
    /** Determine wheter or not the entity linked to the UID is a system NPC. */
    static bool isSysNpc(uint32_t aUID) { return aUID >= SYSNPCID_FIRST && aUID <= SYSNPCID_LAST; }
    /** Determine wheter or not the entity linked to the UID is a user/dynamic NPC. */
    static bool isDynNpc(uint32_t aUID) { return aUID >= DYNANPCID_FIRST && aUID <= DYNANPCID_LAST; }
    /** Determine wheter or not the entity linked to the UID is a monster. */
    static bool isMonster(uint32_t aUID) { return aUID >= MONSTERID_FIRST && aUID <= MONSTERID_LAST; }
    /** Determine wheter or not the entity linked to the UID is a pet. */
    static bool isPet(uint32_t aUID) { return aUID >= PETID_FIRST && aUID <= PETID_LAST; }
    /** Determine wheter or not the entity linked to the UID is a called pet. */
    static bool isCallPet(uint32_t aUID) { return aUID >= CALLPETID_FIRST && aUID <= CALLPETID_LAST; }
    /** Determine wheter or not the entity linked to the UID is a player. */
    static bool isPlayer(uint32_t aUID) { return aUID >= PLAYERID_FIRST && aUID <= PLAYERID_LAST; }

public:
    /** Determine wheter or not the entity is a NPC. */
    bool isNpc() const { return isNpc(mUID); }
    /** Determine wheter or not the entity is a system NPC. */
    bool isSysNpc() const { return isSysNpc(mUID); }
    /** Determine wheter or not the entity is a user/dynamic NPC. */
    bool isDynNpc() const { return isDynNpc(mUID); }
    /** Determine wheter or not the entity is a monster. */
    bool isMonster() const { return isMonster(mUID); }
    /** Determine wheter or not the entity is a pet. */
    bool isPet() const { return isPet(mUID); }
    /** Determine wheter or not the entity is a called pet. */
    bool isCallPet() const { return isCallPet(mUID); }
    /** Determine wheter or not the entity is a player. */
    bool isPlayer() const { return isPlayer(mUID); }

public:
    /* destructor */
    virtual ~Entity();

public:
    /** Send the entity spawn msg. */
    virtual void sendShow(const Player& aPlayer) const = 0;

    /**
     * Update the broadcast set.
     *
     * @param[in]   aClearSet   (Optional) clear the set before updating
     *                          Default is false.
     */
    void updateBroadcastSet(bool aClearSet = false) const;

    /**
     * Clear the broadcast set.
     */
    void clearBroadcastSet() const;

    /**
     * Add an entity to the broadcast set.
     *
     * @param[in]    aEntity    the entity to add
     */
    void addEntityToBCSet(const Entity& aEntity) const;

    /**
     * Remove an entity from the broadcast set.
     *
     * @param[in]    aEntity    the entity to remove
     */
    void removeEntityFromBCSet(const Entity& aEntity) const;

    /**
     * Broadcast a message to all entities of the broadcast set.
     *
     * @param[in]   aMsg           the message to broadcast
     * @param[in]   aIncludeSelf   whether or not the message must be sent to self
     */
    void broadcastRoomMsg(const Msg* aMsg, bool aIncludeSelf) const;

    /** Called when the timer elapse. */
    virtual void timerElapsed() = 0;

public:
    /** Get the entity's UID. */
    uint32_t getUID() const { return mUID; }

    /** Get the entity's name. */
    const char* getName() const { return (mName.empty() ? nullptr : mName.c_str());  }
    /** Get the entity's look/face. */
    uint32_t getLook() const { return mLook; }

    /** Get the entity's map UID. */
    uint32_t getMapId() const { return mMapId; }
    /** Get the entity's X coord. */
    uint16_t getPosX() const { return mPosX; }
    /** Get the entity's Y coord. */
    uint16_t getPosY() const { return mPosY; }
    /** Get the cardinal direction of the entity. */
    uint8_t getDirection() const { return mDirection; }

    /** Set the map UID of the entity. */
    void setMapId(uint32_t aMapId) { mMapId = aMapId; }
    /** Set the position on the map of the entity. */
    void setPosition(uint16_t aPosX, uint16_t aPosY) { mPosX = aPosX; mPosY = aPosY; }
    /** Set the cardinal direction of the entity. */
    void setDirection(uint8_t aDirection) { mDirection = aDirection; }

protected:
    /* constructor */
    Entity(uint32_t aUID);

protected:
    const uint32_t mUID; //!< Entity UID

    std::string mName; //!< Entity name
    uint32_t mLook; //!< Entity look/face

    uint32_t mMapId; //!< Entity map UID
    uint16_t mPosX; //!< Entity X coord.
    uint16_t mPosY; //!< Entity Y coord.
    uint8_t mDirection; //!< Entity cardinal direction

    mutable std::map<uint32_t, const Entity*> mViewSet; //!< set including all entities on screen
    mutable QMutex mViewSetMutex; //!< mutex for accessing the view set
};

#endif // _COPS_V7_EMULATOR_ENTITY_H
