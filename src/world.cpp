/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "world.h"
#include "player.h"
#include "npc.h"
#include "npctask.h"
#include <QThread>

using namespace std;

class Worker : public QThread
{
public:
    Worker() : mTerminate(false) { }
    virtual ~Worker() { }

public slots:
    virtual void terminate()
    {
        mTerminate = true;
        QThread::terminate();
    }

private:
    virtual void run()
    {
        World& world = World::getInstance();

        // TODO: with all entities... maps, items, magics
//        while (!mTerminate)
//        {
//            for (map<int32_t, Player*>::const_iterator
//                    it = world.AllPlayers.begin(), end = world.AllPlayers.end();
//                 !mTerminate && it != end; ++it)
//            {
//                Player* player = it->second;
//                player->timerElapsed(0); // TODO pass time ?

//                yieldCurrentThread();
//            }

//            usleep(250);
//        }
    }

private:
    bool mTerminate;
};

/* static */
World* World::sInstance = nullptr;

/* static */
World&
World :: getInstance()
{
    // TODO? Thread-safe
    if (sInstance == nullptr)
    {
        sInstance = new World();
    }
    return *sInstance;
}

World :: World()
    : AllPlayers(mAllPlayers), AllPlayerNames(mAllPlayerNames),
      AllNPCs(mAllNPCs), AllTasks(mAllTasks)
{
    mWorker = new Worker();
    mWorker->start(QThread::LowPriority);
}

World :: ~World()
{
    mWorker->terminate();
    mWorker->wait(10000); // wait 10s...
    SAFE_DELETE(mWorker);

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

    for (map<uint32_t, NpcTask*>::iterator
            it = mAllTasks.begin(), end = mAllTasks.end();
         it != end; ++it)
    {
        NpcTask* task = it->second;
        SAFE_DELETE(task);
    }
    mAllTasks.clear();
}

bool
World :: addPlayer(Player& aPlayer)
{
    ASSERT_ERR(&aPlayer != nullptr, false);

    bool success = false;

    string name = aPlayer.getName();

    // TODO thread-safe
    if (AllPlayers.end() == AllPlayers.find(aPlayer.getUID()) &&
        AllPlayerNames.end() == AllPlayerNames.find(name))
    {
        AllPlayers[aPlayer.getUID()] = &aPlayer;
        AllPlayerNames[name] = &aPlayer;

        success = true;
    }

    return success;
}

bool
World :: removePlayer(Player& aPlayer)
{
    ASSERT_ERR(&aPlayer != nullptr, false);

    bool success = false;

    string name = aPlayer.getName();

    // TODO thread-safe
    map<uint32_t, Player*>::iterator first_it =
            AllPlayers.find(aPlayer.getUID());
    map<string, Player*>::iterator second_it =
            AllPlayerNames.find(name);

    if (AllPlayers.end() != first_it &&
        AllPlayerNames.end() != second_it)
    {
        AllPlayers.erase(first_it);
        AllPlayerNames.erase(second_it);

        success = true;
    }

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

    // TODO: Thread-safe ?

    bool found = false;
    map<uint32_t, Player*>::const_iterator it;

    if ((it = mAllPlayers.find(aUID)) != mAllPlayers.end())
    {
        *aOutPlayer = it->second;
        found = true;
    }

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

    // TODO: Thread-safe ?

    bool found = false;
    map<string, Player*>::const_iterator it;

    if ((it = mAllPlayerNames.find(aName)) != mAllPlayerNames.end())
    {
        *aOutPlayer = it->second;
        found = true;
    }

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
