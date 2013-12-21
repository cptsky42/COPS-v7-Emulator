/**
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
class Client;
class Player;

class Npc : public Entity
{
    PROHIBIT_COPY(Npc); // constant UID, should be unique...

public:
    // type: 2, 6, 9, 21, 22, 23, 24
    enum Type
    {
        TYPE_NONE = 0,
        TYPE_SHOPKEEPER = 1,
        TYPE_TASK = 2,
        TYPE_STORAGE = 3,
        TYPE_TRUNCK = 4,
        TYPE_FACE = 5,
        TYPE_FROGE = 6, // only used by the client
        TYPE_EMBED = 7,

        _STATUARY_NPC			= 9,			// µñÏñNPC
        _SYNFLAG_NPC			= 10,			// °ïÅÉ±ê¼ÇNPC
        _ROLE_PLAYER			= 11,			// ÆäËûÍæ¼Ò		(only use for client)
        _ROLE_HERO				= 12,			// ×Ô¼º			(only use for client)
        _ROLE_MONSTER			= 13,			// ¹ÖÎï			(only use for client)
        _BOOTH_NPC				= 14,			// °ÚÌ¯NPC		(CBooth class)
        _SYNTRANS_NPC			= 15,			// °ïÅÉ´«ËÍNPC, ¹Ì¶¨ÄÇ¸ö²»ÒªÓÃ´ËÀàÐÍ! (ÓÃÓÚ00:00ÊÕ·Ñ)(LINKIDÎª¹Ì¶¨NPCµÄID£¬ÓëÆäËüÊ¹ÓÃLINKIDµÄ»¥³â)
        _ROLE_BOOTH_FLAG_NPC	= 16,			// Ì¯Î»±êÖ¾NPC	(only use for client)
        _ROLE_MOUSE_NPC			= 17,			// Êó±êÉÏµÄNPC	(only use for client)
        _ROLE_MAGICITEM			= 18,			// ÏÝÚå»ðÇ½		(only use for client)
        _ROLE_DICE_NPC			= 19,			// ÷»×ÓNPC
        _ROLE_SHELF_NPC			= 20,			// ÎïÆ·¼Ü
        _WEAPONGOAL_NPC			= 21,			// ÎäÆ÷°Ð×ÓNPC
        _MAGICGOAL_NPC			= 22,			// Ä§·¨°Ð×ÓNPC
        _BOWGOAL_NPC			= 23,			// ¹­¼ý°Ð×ÓNPC
        _ROLE_TARGET_NPC		= 24,			// °¤´ò£¬²»´¥·¢ÈÎÎñ	(only use for client)
        _ROLE_FURNITURE_NPC		= 25,			// ¼Ò¾ßNPC	(only use for client)
        _ROLE_CITY_GATE_NPC		= 26,			// ³ÇÃÅNPC	(only use for client)
        _ROLE_NEIGHBOR_DOOR		= 27,			// ÁÚ¾ÓµÄÃÅ
        _ROLE_CALL_PET			= 28,			// ÕÙ»½ÊÞ	(only use for client)
        _EUDEMON_TRAINPLACE_NPC	= 29,			// »ÃÊÞÑ±ÑøËù
         _AUCTION_NPC			= 30,			// ÅÄÂòNPC	ÎïÆ·ÁìÈ¡NPC  LW
         _ROLE_MINE_NPC			= 31,			// ¿óÊ¯NPC
    };

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

public:
    /* constructor */
    Npc(uint32_t aUID, const char* aName,
        uint8_t aType, int16_t aLook,
        uint32_t aMapId, uint16_t aPosX, uint16_t aPosY,
        uint8_t aBase, uint8_t aSort);

    /* destructor */
    virtual ~Npc();

public:
    void linkTask(NpcTask* aTask) { ASSERT(aTask != nullptr); mTask = aTask; }
    bool activateNpc(Client& aClient, int32_t aAction);

    /** Send the entity spawn msg. */
    virtual void sendShow(const Player& aPlayer) const;

    /** Called when the timer elapse. */
    virtual void timerElapsed(time_t aTime) { /* TODO */  }

public:
    uint8_t getType() const { return mType; }
    uint8_t getBase() const { return mBase; }
    uint8_t getSort() const { return mSort; }

public:
    bool isShopNpc() const { return mType == TYPE_SHOPKEEPER; }
    // NpcShop* queryShop();

    bool isTaskNpc() const { return mType == TYPE_TASK || (mSort & SORT_TASK) != 0; }
    const NpcTask& queryTask() const { ASSERT(mTask != nullptr); return *mTask; }

    bool isStorageNpc() const { return mType == TYPE_STORAGE; }
    // NpcStorage? queryStorage

private:
    uint8_t mType;
    uint8_t mBase;
    uint8_t mSort;

    NpcTask* mTask;
};

#endif // _COPS_V7_EMULATOR_NPC_H
