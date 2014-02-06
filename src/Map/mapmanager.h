/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MAPMANAGER_H_
#define _COPS_V7_EMULATOR_MAPMANAGER_H_

#include "common.h"
#include "env.h"
#include "gamemap.h"
#include <map>
#include <vector>
#include <QMutex>

class MapData;

/**
 * Global map manager.
 */
class MapManager : public Environment::Global
{
    friend class World; // World must handle DMap packing for initial spawning...

    // !!! class is a singleton !!!
    PROHIBIT_COPY(MapManager);

public:
    /**
     * Get the MapManager singleton. If the object does not exist yet,
     * it will be created.
     *
     * @returns A reference to the singleton
     */
    static MapManager& getInstance();

public:
    /* destructor */
    virtual ~MapManager();

    /**
     * Load all data from the data map files.
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    err_t loadData();

    /**
     * Create a new game map with the specified information.
     *
     * @param[in]  aUID     the unique ID of the map
     * @param[in]  aInfo    a reference to the object containing the info
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    err_t createMap(uint32_t aUID, GameMap::Info** aInfo);

    /**
     * Create a dynamic map based on another map.
     *
     * @param[in]   aUID       the unique ID of the map
     * @param[in]   aRefUID    the unique ID of the reference map
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    err_t linkMap(uint32_t aUID, uint32_t aRefUID);

public:
    /**
     * Get a game map based on its UID.
     *
     * @param[in]   aUID  the map UID
     *
     * @retval The game map object if found
     * @returns NULL otherwise
     */
    GameMap* getMap(uint32_t aUID) const;

private:
    /* constructor */
    MapManager();

    /** MUST NOT BE USED ! Pack all the data and resume the automatic packing. */
    void packAll();
    /** MUST NOT BE USED ! Unpack all the data and suspend the automatic packing. */
    void unpackAll();

private:
    /**
     * Concurrent load of data maps.
     *
     * @param[in]   aMgr     the map manager
     * @param[in]   aWork    the maps to load
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    static err_t loadData(std::map< std::string, std::vector<uint16_t> >* aWork);

private:
    static MapManager* sInstance; //!< static instance of the singleton

private:
    std::map<uint32_t, GameMap*> mGameMaps; //!< all game maps
    std::map<uint16_t, MapData*> mMaps; //!< all map data based on the UID
    std::map<std::string, MapData*> mData; //!< all map data based on the file

    QMutex mWorkMutex; //!< mutex for the work map
    QMutex mDataMutex; //!< mutex for the data (others maps)
};

#endif // _COPS_V7_EMULATOR_MAPMANAGER_H_
