/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "mapmanager.h"
#include "gamemap.h"
#include "mapdata.h"
#include "finder.h"
#include "inifile.h"
#include <stdio.h>
#include <QtConcurrentRun>
#include <QThread>

using namespace std;

/* static */
MapManager* MapManager::sInstance = nullptr;

/* static */
MapManager&
MapManager :: getInstance()
{
    static volatile long protect = 0;

    if (sInstance == nullptr)
    {
        if (1 == atomic_inc(&protect))
        {
            // create the instance
            sInstance = new MapManager();
        }
        else
        {
            while (sInstance == nullptr)
                QThread::yieldCurrentThread();
        }
    }
    return *sInstance;
}

MapManager :: MapManager()
{

}

MapManager :: ~MapManager()
{
    for (map<uint32_t, GameMap*>::iterator
            it = mGameMaps.begin(), end = mGameMaps.end();
         it != end; ++it)
    {
        GameMap* map = it->second;
        SAFE_DELETE(map);
    }
    mGameMaps.clear();


    for (map<string, MapData*>::iterator
            it = mData.begin(), end = mData.end();
         it != end; ++it)
    {
        MapData* data = it->second;
        SAFE_DELETE(data);
    }
    mMaps.clear(); // refs to data objects...
    mData.clear();
}

err_t
MapManager :: loadData()
{
    ASSERT_ERR(mGameMaps.empty() && mMaps.empty() && mData.empty(), ERROR_INVALID_STATE);

    err_t err = ERROR_SUCCESS;

    const char* path = "./GameMap.ini";
    if (Finder::fileExists(path))
    {
        IniFile gamemap;
        err = gamemap.open(path);

        vector<string> sections;
        gamemap.getSections(sections);

        int numCPU = getNumCPU();
        LOG(INFO, "Detected %d core(s). Spawning %d workers for loading maps...",
            numCPU, numCPU);

        map< string, vector<uint16_t> > maps;
        for (vector<string>::const_iterator
                it = sections.begin(), end = sections.end();
             ERROR_SUCCESS == err && it != end; ++it)
        {
            const string& section = *it;
            unsigned int mapId = 0;

            if (sscanf(section.c_str(), "Map%u", &mapId) == 1)
            {
                if (mMaps.find((uint16_t)mapId) == mMaps.end())
                {
                    string dataPath = gamemap.readString(section + "/File", "N/A");
                    if (dataPath != "N/A")
                    {
                        map< string, vector<uint16_t> >::iterator maps_it;
                        if ((maps_it = maps.find(dataPath)) == maps.end())
                        {
                            vector<uint16_t> mapIds; mapIds.push_back((uint16_t)mapId);
                            maps.insert(maps_it, pair< string, vector<uint16_t> >(dataPath, mapIds));
                        }
                        else
                            maps_it->second.push_back((uint16_t)mapId);
                    }
                    else
                    {
                        LOG(WARN, "Could not find the key 'File' under the group '%s'. Skipping",
                            section.c_str());
                    }
                }
                else
                {
                    LOG(ERROR, "Duplicated entry for map %u.", mapId);
                    err = ERROR_CANNOT_CREATE;
                }
            }
            else
            {
                LOG(WARN, "Found an invalid group (%s) at root. Skipping.",
                    section.c_str());
            }
        }

        vector< QFuture<err_t> > errors;
        for (int i = 0; ERROR_SUCCESS == err && i < numCPU; ++i)
        {
            QFuture<err_t> error = QtConcurrent::run(MapManager::loadData, &maps);
            errors.push_back(error);
        }

        for (vector< QFuture<err_t> >::const_iterator
                it = errors.begin(), end = errors.end();
             ERROR_SUCCESS == err && it != end; ++it)
        {
            err = (*it).result();
        }
    }
    else
    {
        LOG(ERROR, "Could not find the '%s' file for loading maps.", path);
        err = ERROR_FILE_NOT_FOUND;
    }

    return err;
}

/* static */
err_t
MapManager :: loadData(map< string, vector<uint16_t> >* aWork)
{
    ASSERT_ERR(aWork != nullptr, ERROR_INVALID_POINTER);

    MapManager& mgr = MapManager::getInstance();

    err_t err = ERROR_SUCCESS;

    LOG(INFO, "Worker %u starting.", QThread::currentThreadId());

    while (IS_SUCCESS(err))
    {
        mgr.mWorkMutex.lock();

        if (aWork->empty())
        {
            mgr.mWorkMutex.unlock();
            break;
        }

        map< string, vector<uint16_t> >::iterator work_it = aWork->begin();
        ASSERT(work_it != aWork->end());

        string dataPath(work_it->first);
        vector<uint16_t> mapIds; mapIds.swap(work_it->second); // swap O(1)
        aWork->erase(work_it);

        mgr.mWorkMutex.unlock();

        MapData* data = nullptr;
        DOIF(err, MapData::load(&data, dataPath.c_str()));

        if (IS_SUCCESS(err))
        {
            ASSERT_ERR(data != nullptr, ERROR_INVALID_POINTER);

            LOG(INFO, "Loaded map data at '%s'.",
                dataPath.c_str());

            LOG(DBG, "Compressing map data of '%s'...",
                dataPath.c_str());
            err = data->pack();

            if (IS_SUCCESS(err))
            {
                LOG(INFO, "Compressed map data of '%s'...",
                    dataPath.c_str());

                mgr.mDataMutex.lock();

                mgr.mData[dataPath] = data;
                for (vector<uint16_t>::const_iterator
                        it = mapIds.begin(), end = mapIds.end();
                     it != end; ++it)
                {
                    uint16_t mapId = *it;
                    LOG(INFO, "Found already loaded map data for id=%u. Using %s.",
                        mapId, dataPath.c_str());

                    mgr.mMaps[(uint16_t)mapId] = data;
                }
                data = nullptr;

                mgr.mDataMutex.unlock();
            }
            else
            {
                LOG(ERROR, "Could not compress the map data file '%s'. Ignoring error.",
                    dataPath.c_str());
                err = ERROR_SUCCESS;
            }
        }
        else if (ERROR_FILE_NOT_FOUND == err)
        {
            LOG(WARN, "Could not find all files for loading the map data file '%s'. Ignoring error.",
                dataPath.c_str());
            err = ERROR_SUCCESS;
        }
        SAFE_DELETE(data);

        QThread::yieldCurrentThread();
    }

    LOG(INFO, "Worker %u done. (err=%d)", QThread::currentThreadId(), err);

    return err;
}

err_t
MapManager :: createMap(uint32_t aUID, GameMap::Info** aInfo)
{
    ASSERT_ERR(aInfo != nullptr && *aInfo != nullptr, ERROR_INVALID_POINTER);

    err_t err = ERROR_SUCCESS;

    if (mGameMaps.find(aUID) == mGameMaps.end())
    {
        map<uint16_t, MapData*>::const_iterator it;
        if ((it = mMaps.find((*aInfo)->DocID)) != mMaps.end())
        {
            GameMap* gameMap = new GameMap(aUID, aInfo, *it->second);
            ASSERT(gameMap != nullptr);

            mGameMaps[aUID] = gameMap;

            LOG(INFO, "Created game map %d with data of %u.",
                gameMap->getUID(), gameMap->getDocID());
        }
        else
        {
            LOG(ERROR, "Missing map data for doc ID %u. The map %d won't be created.",
                (*aInfo)->DocID, aUID);
            err = ERROR_NOT_FOUND;
        }
    }
    else
    {
        LOG(ERROR, "Duplicated map %d.", aUID);
        err = ERROR_CANNOT_CREATE;
    }

    return err;
}

GameMap*
MapManager :: getMap(uint32_t aUID) const
{
    GameMap* gameMap = nullptr;

    map<uint32_t, GameMap*>::const_iterator it;
    if ((it = mGameMaps.find(aUID)) != mGameMaps.end())
    {
        gameMap = it->second;
    }

    return gameMap;
}

void
MapManager :: packAll()
{
    mDataMutex.lock();

    for (map<string, MapData*>::iterator
            it = mData.begin(), end = mData.end();
         it != end; ++it)
    {
        MapData* map = it->second;
        map->resumePacking();
        map->pack(nullptr);
    }

    mDataMutex.unlock();
}

void
MapManager :: unpackAll()
{
    mDataMutex.lock();

    for (map<string, MapData*>::iterator
            it = mData.begin(), end = mData.end();
         it != end; ++it)
    {
        MapData* map = it->second;
        map->unpack(nullptr);
        map->suspendPacking();
    }

    mDataMutex.unlock();
}
