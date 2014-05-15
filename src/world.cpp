/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "world.h"
#include "database.h"
#include "mapmanager.h"
#include "player.h"
#include "npc.h"
#include "script.h"
#include "generator.h"
#include <QThread>
#include <QtConcurrentRun>

using namespace std;

///////////////////////////////////////////////////////////////
////// World
///////////////////////////////////////////////////////////////

/* static */
World* World::sInstance = nullptr;

/* static */
World&
World :: getInstance()
{
    static volatile long protect = 0;

    if (sInstance == nullptr)
    {
        if (1 == atomic_inc(&protect))
        {
            // create the instance
            sInstance = new World();
        }
        else
        {
            while (sInstance == nullptr)
                QThread::yieldCurrentThread();
        }
    }

    return *sInstance;
}

World :: World()
    : mGenWorkerRunning(false),
      mLastMonsterUID(Entity::MONSTERID_FIRST - 1),
      mStopping(false)
{
    mWorkers.push_back(QtConcurrent::run(&World::processPlayers));
}

World :: ~World()
{
    mStopping = true;
    for (vector< QFuture<void> >::iterator
            it = mWorkers.begin(), end = mWorkers.end();
         it != end; ++it)
    {
        QFuture<void>& future = *it;
        future.waitForFinished();
    }
    mWorkers.clear();

    // clients will free the players...
    mAllPlayers.clear();
    mAllPlayerNames.clear();

    for (map<uint32_t, Npc*>::iterator
            it = mAllNPCs.begin(), end = mAllNPCs.end();
         it != end; ++it)
    {
        Npc* npc = it->second;
        SAFE_DELETE(npc);
    }
    mAllNPCs.clear();

    for (map<uint32_t, Script*>::iterator
            it = mAllTasks.begin(), end = mAllTasks.end();
         it != end; ++it)
    {
        Script* task = it->second;
        SAFE_DELETE(task);
    }
    mAllTasks.clear();

    for (vector<Generator*>::iterator
            it = mAllGenerators.begin(), end = mAllGenerators.end();
         it != end; ++it)
    {
        Generator* generator = *it;
        SAFE_DELETE(generator);
    }
    mAllGenerators.clear();
}

bool
World :: addPlayer(Player& aPlayer)
{
    ASSERT_ERR(&aPlayer != nullptr, false);

    bool success = false;

    string name = aPlayer.getName();

    mPlayerMutex.lock();
    if (mAllPlayers.end() == mAllPlayers.find(aPlayer.getUID()) &&
        mAllPlayerNames.end() == mAllPlayerNames.find(name))
    {
        mAllPlayers[aPlayer.getUID()] = &aPlayer;
        mAllPlayerNames[name] = &aPlayer;

        success = true;
    }
    mPlayerMutex.unlock();

    return success;
}

bool
World :: removePlayer(Player& aPlayer)
{
    ASSERT_ERR(&aPlayer != nullptr, false);

    bool success = false;

    string name = aPlayer.getName();

    mPlayerMutex.lock();

    map<uint32_t, Player*>::iterator first_it =
            mAllPlayers.find(aPlayer.getUID());
    map<string, Player*>::iterator second_it =
            mAllPlayerNames.find(name);

    if (mAllPlayers.end() != first_it &&
        mAllPlayerNames.end() != second_it)
    {
        mAllPlayers.erase(first_it);
        mAllPlayerNames.erase(second_it);

        success = true;
    }

    mPlayerMutex.unlock();

    return success;
}

bool
World :: queryEntity(Entity** aOutEntity, uint32_t aUID) const
{
    ASSERT_ERR(aOutEntity != nullptr && *aOutEntity == nullptr, false);

    bool found = false;

    if (Entity::isPlayer(aUID))
    {
        Player* player = nullptr;
        found = queryPlayer(&player, aUID);
        *aOutEntity = player;
    }
    else if (Entity::isNpc(aUID))
    {
        Npc* npc = nullptr;
        found = queryNpc(&npc, aUID);
        *aOutEntity = npc;
    }
    else
    {
        // not implemented yet...
        ASSERT_ERR(false, false);
    }

    return found;
}

bool
World :: queryPlayer(Player** aOutPlayer, uint32_t aUID) const
{
    ASSERT_ERR(aOutPlayer != nullptr && *aOutPlayer == nullptr, false);
    ASSERT_ERR(Entity::isPlayer(aUID), false);

    bool found = false;
    map<uint32_t, Player*>::const_iterator it;

    mPlayerMutex.lock();
    if ((it = mAllPlayers.find(aUID)) != mAllPlayers.end())
    {
        *aOutPlayer = it->second;
        found = true;
    }
    mPlayerMutex.unlock();

    return found;
}

bool
World :: queryPlayer(Player** aOutPlayer, const char* aName) const
{
    ASSERT_ERR(aName != nullptr && aName[0] != '\0', false);

    string name = aName;
    return queryPlayer(aOutPlayer, name);
}
bool
World :: queryPlayer(Player** aOutPlayer, const std::string& aName) const
{
    ASSERT_ERR(aOutPlayer != nullptr && *aOutPlayer == nullptr, false);
    ASSERT_ERR(!aName.empty(), false);

    bool found = false;
    map<string, Player*>::const_iterator it;

    mPlayerMutex.lock();
    if ((it = mAllPlayerNames.find(aName)) != mAllPlayerNames.end())
    {
        *aOutPlayer = it->second;
        found = true;
    }
    mPlayerMutex.unlock();

    return found;
}

bool
World :: queryNpc(Npc** aOutNpc, uint32_t aUID) const
{
    ASSERT_ERR(aOutNpc != nullptr && *aOutNpc == nullptr, false);
    ASSERT_ERR(Entity::isNpc(aUID), false);

    // TODO: Thread-safe ?

    bool found = false;
    map<uint32_t, Npc*>::const_iterator it;

    if ((it = mAllNPCs.find(aUID)) != mAllNPCs.end())
    {
        *aOutNpc = it->second;
        found = true;
    }

    return found;
}

bool
World :: queryTask(Script** aOutTask, uint32_t aUID) const
{
    ASSERT_ERR(aOutTask != nullptr && *aOutTask == nullptr, false);

    bool found = false;
    map<uint32_t, Script*>::const_iterator it;

    mTaskMutex.lock();
    if ((it = mAllTasks.find(aUID)) != mAllTasks.end())
    {
        *aOutTask = it->second;
        found = true;
    }
    mTaskMutex.unlock();

    return found;
}

Monster*
World :: generateMonster(uint32_t aId, Generator* aGenerator)
{
    static const Database& db = Database::getInstance(); // singleton

    Monster* monster = nullptr;
    const Monster::Info* info = nullptr;

    if (IS_SUCCESS(db.getMonsterInfo(&info, aId)))
    {
        uint32_t uid = 0;

        if (mLastMonsterUID < Entity::MONSTERID_LAST)
        {
            uid = ++mLastMonsterUID;
        }
        else if (!mRecycledMonsterUIDs.empty())
        {
            mUIDMutex.lock();

            uid = mRecycledMonsterUIDs.front();
            mRecycledMonsterUIDs.pop();

            mUIDMutex.unlock();
        }
        else
        {
            LOG(ERROR, "Using all monster UIDs !");
            fprintf(stderr, "Using all monster UIDs !\n");
            abort();
        }

        monster = new Monster(uid, *info, aGenerator);
    }

    return monster;
}

void
World :: recycleMonsterUID(uint32_t aUID)
{
    mUIDMutex.lock();

    mRecycledMonsterUIDs.push(aUID);

    mUIDMutex.unlock();
}

///////////////////////////////////////////////////////////////
////// Workers
///////////////////////////////////////////////////////////////

void
World :: startMonstersRegeneration()
{
    ASSERT(!mGenWorkerRunning);

    mGenWorkerRunning = true;
    mWorkers.push_back(QtConcurrent::run(&World::regenerateMonsters));
}

/* static */
void
World :: regenerateMonsters()
{
    const int32_t MAXNPC_PER_ONTIMER = 20;

    World& world = World::getInstance();
    MapManager& mgr = MapManager::getInstance();
    int32_t maxNpc = MAXNPC_PER_ONTIMER;
    size_t index = 0;
    bool initializing = true;

    LOG(INFO, "Worker %u starting for handling generators.",
        QThread::currentThreadId());

    // unpack all data for initial spawning !!!
    mgr.unpackAll();

    index = 0;
    while (!world.mStopping)
    {
        world.mGeneratorMutex.lock();

        maxNpc = MAXNPC_PER_ONTIMER;
        if (index >= world.mAllGenerators.size())
            index = 0;

        Generator* generator = nullptr;
        for (size_t count = world.mAllGenerators.size();
             !world.mStopping && index < count; ++index)
        {
            generator = world.mAllGenerators[index];

            maxNpc -= generator->generate(maxNpc, initializing);
            if (maxNpc <= 0)
                break;

            QThread::yieldCurrentThread();
        }

        if (index >= world.mAllGenerators.size())
            index = 0;

        world.mGeneratorMutex.unlock();

        if (initializing &&
            index == 0 && MAXNPC_PER_ONTIMER == maxNpc) // SQL done and finished the initial spawning
        {
            initializing = false;
            mgr.packAll(); // done intial spawning... repack everything

            LOG(INFO, "Initial spawning of monsters completed...");
            fprintf(stdout, "Initial spawning of monsters completed...\n");
        }

        mssleep(20);
    }
}

/* static */
void
World :: processPlayers()
{
    World& world = World::getInstance();

    LOG(INFO, "Worker %u starting for processing players.",
        QThread::currentThreadId());

    while (!world.mStopping)
    {
        // TODO : limit the number of player to process
        world.mPlayerMutex.lock();
        for (map<uint32_t, Player*>::const_iterator
             it = world.mAllPlayers.begin(), end = world.mAllPlayers.end();
             !world.mStopping && it != end; ++it)
        {
            Player* player = it->second;
            player->timerElapsed();

            QThread::yieldCurrentThread();
        }
        world.mPlayerMutex.unlock();

        mssleep(20);
    }
}
