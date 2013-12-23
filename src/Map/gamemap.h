/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_GAMEMAP_H_
#define _COPS_V7_EMULATOR_GAMEMAP_H_

#include "common.h"
#include "mapbase.h"
#include "mapdata.h"
#include <math.h>
#include <map>
#include <algorithm>

class Client;
class Entity;
class Player;

/**
 * A game map object which is linked to a MapData object.
 * It contains and handles all entities on the map.
 */
class GameMap
{
    friend class MapManager;

public:
    /**
     * List of all flag for the map's type.
     */
    enum Flags
    {
        /** The map is normal. */
        TYPE_NORMAL             = 0x0000,
        /** The map is a Pk field. Players won't get Pk points and their name won't flash. */
        TYPE_PK_FIELD           = 0x0001,
        /** The map disable changing map with magic call (team member) ??? */
        TYPE_CHGMAP_DISABLE     = 0x0002, // magic call team member
        /** The map doesn't save the position. It saves the previous map. */
        TYPE_RECORD_DISABLE     = 0x0004,
        /** The map doesn't allow Pk. */
        TYPE_PK_DISABLE         = 0x0008,
        /** The map allows booth creation. */
        TYPE_BOOTH_ENABLE       = 0x0010,
        /** The map doesn't allow teams. */
        TYPE_TEAM_DISABLE       = 0x0020,
        /** The map doesn't allow changing map by action. (Scrolls, etc) */
        TYPE_TELEPORT_DISABLE	= 0x0040, // chgmap by action
        /** The map is a syndicate map. */
        TYPE_SYN_MAP            = 0x0080,
        /** The map is a prison map. */
        TYPE_PRISON_MAP         = 0x0100,
        /** The map doesn't allow flying. */
        TYPE_WING_DISABLE       = 0x0200, // bowman fly disable
        /** The map is a family map. */
        TYPE_FAMILY             = 0x0400,
        /** The map is a mine field. */
        TYPE_MINE_FIELD         = 0x0800,
        // TODO
        TYPE_PK_GAME            = 0x1000, // or CALLNEWBIE_DISABLE ?
        TYPE_NEVER_WOUND        = 0x2000, // or REBORN_NOW_ENABLE ?
        TYPE_DEAD_ISLAND        = 0x4000  // or NEWBIE_PROTECT ?
    };

public:
    /**
     * The specific information of a game map.
     * Mostly loaded from the database.
     */
    struct Info
    {
    public:
        /** The unique ID of the MapData */
        uint16_t DocID;
        /** The type (bitfield) of the map. */
        uint32_t Type;
        /** The owner UID of the map. */
        uint32_t OwnerUID;
        /** The main portal X-coord. It is used for reborn. */
        uint16_t PortalX;
        /** The main portal Y-coord. It is used for reborn. */
        uint16_t PortalY;
        /** The reborn map UID. */
        uint32_t RebornMap;
        /** The reborn portal UID. Zero corresponds to the main portal. */
        int32_t RebornPortal;
        /** The light in ARGB code. */
        uint32_t Light;
    };

public:
    /** The unique ID of the prison map. */
    static const uint32_t PRISON_MAP_UID = 10000; // TODO
    /** The unique ID of the newbie map. */
    static const uint32_t NEWBIE_MAP_UID = 10001; // TODO
    /** The first valid unique ID for dynamic maps. */
    static const uint32_t DYNAMIC_MAP_UID = 1000000;

public:
    static inline int distance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
    { return std::max(abs((int)x1 - (int)x2), abs((int)y1 - (int)y2)); }

public:
    /* destructor */
    ~GameMap();

public:
    /** Get the map's UID. */
    uint32_t getUID() const { return mUID; }
    /** Get the map's owner's UID. */
    uint32_t getOwnerUID() const { return mInfo->OwnerUID; }

    /** Get the map's width in term of cells count. */
    uint16_t getWidth() const { return mData.getWidth(); }
    /** Get the map's height in term of cells count. */
    uint16_t getHeight() const { return mData.getHeight(); }

    /** Determine whether or not the cell is accessible. */
    bool getFloorAccess(uint16_t aPosX, uint16_t aPosY) const;
    /** Determine whether or nor the cell altitude is accessible. */
    int16_t getFloorAlt(uint16_t aPosX, uint16_t aPosY) const;

    /** Get the map's document ID which corresponds to the UID of the MapData. */
    uint16_t getDocID() const { return mInfo->DocID; }
    /** Get the map's type flags. */
    uint32_t getType() const { return mInfo->Type; }
    /** Get the map's light in ARGB code. */
    uint32_t getLight() const { return mInfo->Light; }

    //	int		GetWidthOfBlock()				{ return (m_pMapData->GetMapWidth()-1) / CELLS_PER_BLOCK + 1; }
    //	int		GetHeightOfBlock()				{ return (m_pMapData->GetMapHeight()-1) / CELLS_PER_BLOCK + 1; }

    //	OBJID	GetSynID()						{ if(GetOwnerType() == OWNER_SYN) return GetOwnerID(); return ID_NONE; }
    //	DWORD	GetStatus()						{ return m_nStatus; }
    //	DWORD	GetType()						{ return m_pData->GetInt(GAMEMAPDATA_TYPE); }
    //	POINT	GetPortal()						{ POINT pos; pos.x=m_pData->GetInt(GAMEMAPDATA_PORTAL0_X); pos.y=m_pData->GetInt(GAMEMAPDATA_PORTAL0_Y); return pos;}

public:
    /** Determine whether or not the cell is valid for laying an item. */
    bool isLayItemEnable(uint16_t aPosX, uint16_t aPosY) const { return getFloorAccess(aPosX, aPosY); }
    /** Determine whether or not the coords are in the map' zone's limits. */
    bool isValidPoint(uint16_t aPosX, uint16_t aPosY) const { return (aPosX < getWidth() && aPosY < getHeight()); }
    /** Determine whether or not the cell is valid a player. */
    bool isStandEnable(uint16_t aPosX, uint16_t aPosY) const { return getFloorAccess(aPosX, aPosY); }
    /** Determine whether or not it is a newbie map. */
    bool isNewbieMap() const { return NEWBIE_MAP_UID == mUID; }
    /** Determine whether or not it is a dynamic map. */
    bool isDynaMap() const { return mUID >= DYNAMIC_MAP_UID; }
    /** Determine whether or not it is a training map. */
    bool isTrainMap() const { return 1039 == mUID; }
    /** Determine whether or not it is a Pk field. */
    bool isPkField() const { return (mInfo->Type & TYPE_PK_FIELD) != 0; }
    /** Determine whether or not changing map by maigc is disabled. */
    bool isChgMapDisabled() const { return (mInfo->Type & TYPE_CHGMAP_DISABLE) != 0; }
    /** Determine whether or not the current map can be saved. */
    bool isRecordDisabled() const { return (mInfo->Type & TYPE_RECORD_DISABLE) != 0; }
    /** Determine whether or not the Pk is disabled. */
    bool isPkDisabled() const { return (mInfo->Type & TYPE_PK_DISABLE) != 0; }
    /** Determine whether or not the teams are disabled. */
    bool isTeamDisabled() const { return (mInfo->Type & TYPE_TEAM_DISABLE) != 0; }
    /** Determine whether or not changing map by action is disabled. */
    bool isTeleportDisabled() const { return (mInfo->Type & TYPE_TELEPORT_DISABLE) != 0; }
    /** Determine whether or not the map is a syndicate map. */
    bool isSynMap() const { return (mInfo->Type & TYPE_SYN_MAP) != 0; }
    /** Determine whether or not the map is a prison map. */
    bool isPrisonMap() const { return (mInfo->Type & TYPE_PRISON_MAP) != 0; }
    /** Determine whether or not flying is disabled. */
    bool isWingDisabled() const { return (mInfo->Type & TYPE_WING_DISABLE) != 0; }
    /** Determine whether or not the map is a mine map. */
    bool isMineField() const { return (mInfo->Type & TYPE_MINE_FIELD) != 0; }
    /** Determine whether or not the map is a game map. */
    bool isPkGameMap() const { return (mInfo->Type & TYPE_PK_GAME) != 0; }
    /** Determine whether or not the map is a family map. */
    bool isFamilyMap() const { return (mInfo->Type & TYPE_FAMILY) != 0; }
    /** Determine whether or not booths are enabled. */
    bool isBoothEnabled() const { return (mInfo->Type & TYPE_BOOTH_ENABLE) != 0; }
    /** Determine whether or not the a war is active on the map. */
    bool isWarTime() const { return false; /* (getStatus() & STATUS_WAR) != 0 */ } // TODO


public:
    void sendMapInfo(const Player& aPlayer) const;
    void sendBlockInfo(const Player& aPlayer) const;

    void updateBroadcastSet(const Entity& aEntity) const;

    /**
     * Add a new entity on the map.
     *
     * @param[in]   aEntity     the entity to add
     */
    void enterRoom(Entity& aEntity);

    /**
     * Remove an entity from the map.
     *
     * @param[in]   aEntity     the entity to remove
     */
    void leaveRoom(Entity& aEntity);

private:
    /* constructor */
    GameMap(uint32_t aUID, Info** aInfo, MapData& aData);

private:
    const uint32_t mUID; //!< The map's unique ID.
    Info* mInfo; //!< The map's information.
    MapData& mData; //!< The map's data.

    std::map<uint32_t, Entity*> mEntities; //!< the entities on the map
};

#endif // _COPS_V7_EMULATOR_GAMEMAP_H_

//	bool	QueryObjInPos(int nPosX, int nPosY, OBJID idObjType, void** ppObj);
//	bool	QueryObj(int nCenterX, int nCenterY, OBJID idObjType, OBJID idObj, void** ppObj);
//	IRole*	QueryRole(int nCenterX, int nCenterY, OBJID idObj);
//	IRole*	QueryRoleByPos(int nPosX, int nPosY);
////	IMapData*	QueryMapData()							{ CHECKF(m_pMapData); return m_pMapData; }
//	CWeatherRegion*	QueryWeatherRegion(OBJID id)		{ CHECKF(m_setWeather && id != ID_NONE); return m_setWeather->GetObj(id);}

//public: // block
//	CGameBlock&	QueryBlock(int nPosX, int nPosY)		{ return m_setBlock[Block(nPosX)][Block(nPosY)]; }
//	IMapThing*	QueryThingByIndex(int x, int y, int z)	{ return BlockByIndex(x,y).QuerySet()->GetObjByIndex(z); }
//	CGameBlock&	BlockByIndex(int x, int y)				{ return m_setBlock[x][y]; }		// call by FOR_9_xxx
//protected:
//	IRegionSet*	QueryRegionSet()						{ CHECKF(m_setRegion); return m_setRegion; }

//public: // block info
//	virtual void	SendBlockInfo(IRole* pRole);		// ²»°üº¬×Ô¼º
//	virtual void	BroadcastBlockMsg(IMapThing* pThing, CNetMsg* pMsg, bool bSendSelf = false);
//	virtual void	BroadcastBlockMsg(int nPosX, int nPosY, CNetMsg* pMsg);

//public: // role
//	void	MoveTo(IRole* pRole, int nNewPosX, int nNewPosY, BOOL bLeaveBlock=false, BOOL bEnterBlock=false);		// ÓÐ¿ÉÄÜÒÆ¶¯µ½ÏàÁÚµÄBLOCK
//	void	IncRole(int x, int y)		{ m_pMapData->IncRole(x, y); }
//	void	DecRole(int x, int y)		{ m_pMapData->DecRole(x, y); }		// normal use LeaveRoom or MoveTo but dead

//public: // region info
//	void	SendRegionInfo(CUser* pUser);
//	void	ClearRegionInfo(CUser* pUser);
//	void	ChangeRegion(CUser* pUser, int nNewPosX, int nNewPosY);

//public: // region -----------------------------
//	CRegionData*	QueryRegion(int nRegionType, int x, int y);
//	bool	IsInRegionType(int nRegionType, int x, int y);
//	static bool	IsInRegion(CRegionData* pData, OBJID idMap, int x, int y);
//	bool	BroadcastDance(CUser* pUser, OBJID idEmotion);

//public: // application
//	void	CollectMapThing(MAPTHING_SET& psetMapThing, const POINT pos, int nRange, OBJID idObjTypeUnion);	// idObjTypeUnion Ö§³Ö¶àÀàÐÍ¶ÔÏó
//	bool	FindDropItemCell(int nRange, POINT* pos);			// pos: in/out
//	bool	GetPassageMap(OBJID* pidMap, POINT* pposTarget, const POINT& pos);
//	bool	GetRebornMap(OBJID* pidMap, POINT* pposTarget);
//	bool	SetStatus(int nStatus, bool flag);				// return false: no change
//	void	SetSynID(OBJID idSyn, bool bWithAllNpc);				// true: set all syna npc syn id, yet.
//	void	SetUserID(OBJID idUser, bool bWithAllNpc);				// true: set all syna npc syn id, yet.
//	bool	EraseMap();
//	bool	AddTerrainObj(OBJID idOwner, int x, int y, OBJID idTerrainObj);
//	bool	DelTerrainObj(OBJID idOwner);
