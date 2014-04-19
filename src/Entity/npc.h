/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_NPC_H
#define _COPS_V7_EMULATOR_NPC_H

#include "common.h"
#include "entity.h"
#include <string>

class NpcTask;
class NpcShop;
class Client;
class Player;

/**
 * Base class of all NPC (but not monsters).
 */
class Npc : public Entity
{
    PROHIBIT_COPY(Npc); // constant UID, should be unique...

public:
    /** The type of the NPC. */
    enum Type
    {
        /** No type (invalid). */
        TYPE_NONE = 0,
        /** The NPC is a shop. */
        TYPE_SHOPKEEPER = 1,
        /** The NPC is a task NPC (normal). */
        TYPE_TASK = 2,
        /** The NPC is a storage NPC. */
        TYPE_STORAGE = 3,
        /** The NPC is a trunck NPC. */
        TYPE_TRUNCK = 4,
        /** The NPC can change the face. */
        TYPE_FACE = 5,
        /** The NPC can forge items. */
        TYPE_FORGE = 6, // only used by the client
        /** The NPC can embed gems in items. */
        TYPE_EMBED = 7
    };

    /** The sort of the NPC. */
    enum Sort
    {
        SORT_NONE = 0,
        SORT_TASK = 1,
        SORT_RECYCLE = 2,
        SORT_SCENE = 4,
        SORT_LINKMAP = 8,
        SORT_DIEACTION = 16,
        SORT_DELENABLE = 32,
        SORT_EVENT = 64,
        SORT_TABLE = 128,

        SORT_USELINKID = SORT_LINKMAP | SORT_DIEACTION | SORT_EVENT
    };

    /** Position of the SQL data in the result set. */
    enum SqlData
    {
        SQLDATA_ID = 0,
        SQLDATA_NAME,
        SQLDATA_TYPE,
        SQLDATA_LOOKFACE,
        SQLDATA_MAPID,
        SQLDATA_CELLX,
        SQLDATA_CELLY,
        SQLDATA_LIFE,
        SQLDATA_MAX_LIFE,
        SQLDATA_BASE,
        SQLDATA_SORT
    };

public:
    /* constructor */
    Npc(uint32_t aUID, const char* aName,
        uint8_t aType, uint16_t aLook,
        uint32_t aMapId, uint16_t aPosX, uint16_t aPosY,
        uint8_t aBase, uint8_t aSort);

    /* destructor */
    virtual ~Npc();

public:
    /**
     * Link a task to the NPC.
     *
     * @param[in]    aTask    the new task of the NPC.
     */
    void linkTask(NpcTask* aTask) { ASSERT(aTask != nullptr); mTask = aTask; }

    /**
     * Try to active the NPC.
     *
     * @param[in]   aClient     the client requesting the activation
     * @param[in]   aAction     the action
     *
     * @retval TRUE on success
     * @returns FALSE otherwise
     */
    bool activateNpc(Client& aClient, int32_t aAction);

    /** Send the entity spawn msg. */
    virtual void sendShow(const Player& aPlayer) const;

    /** Called when the timer elapse. */
    virtual void timerElapsed() { /* TODO npc timerElapsed */  }

public:
    /** Get the type of the NPC. */
    uint8_t getType() const { return mType; }
    /** Get the base of the NPC. */
    uint8_t getBase() const { return mBase; }
    /** Get the sort of the NPC. */
    uint8_t getSort() const { return mSort; }

public:
    /** Determine whether or not the NPC is a shop NPC. */
    bool isShopNpc() const { return mType == TYPE_SHOPKEEPER; }
    /** Get the shop linked to the NPC. */
    const NpcShop& queryShop() const;

    /** Determine whether or not the NPC is a task NPC. */
    bool isTaskNpc() const { return mType == TYPE_TASK || (mSort & SORT_TASK) != 0; }
    /** Get the task linked to the NPC. */
    const NpcTask& queryTask() const { ASSERT(mTask != nullptr); return *mTask; }

    /** Determine whether or not the NPC is a storage NPC. */
    bool isStorageNpc() const { return mType == TYPE_STORAGE; }
    // NpcStorage? queryStorage

private:
    uint8_t mType; //!< the type of the NPC
    uint8_t mBase; //!< the base of the NPC
    uint8_t mSort; //!< the sort of the NPC

    NpcTask* mTask; //!< the task of the NPC
};

#endif // _COPS_V7_EMULATOR_NPC_H
