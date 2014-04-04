/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "entity.h"
#include "player.h"
#include "gamemap.h"
#include "mapmanager.h"
#include "msgaction.h"

using namespace std;

Entity :: Entity(uint32_t aUID)
    : mUID(aUID),
      mName("Unknown"), mLook(0),
      mMapId(0), mPosX(0), mPosY(0), mDirection(0)
{

}

Entity :: ~Entity()
{

}

void
Entity :: updateBroadcastSet(bool aClearSet) const
{
    if (aClearSet)
    {
        clearBroadcastSet();
    }

    const MapManager& mgr = MapManager::getInstance();
    GameMap* map = mgr.getMap(mMapId);

    if (map != nullptr)
    {
        map->updateBroadcastSet(*this);
    }
}

void
Entity :: clearBroadcastSet() const
{
    mViewSetMutex.lock();

    map<uint32_t, const Entity*>::iterator it;
    while (!mViewSet.empty())
    {
        it = mViewSet.begin();

        // remove the current entity from the other screen
        const Entity& entity = *it->second;
        entity.removeEntityFromBCSet(*this);

        // remove the entity from this current set
        mViewSet.erase(it);

        // remove the entity from the current screen
        if (isPlayer())
        {
            Player* player = (Player*)this;

            MsgAction msg(&entity, 0, MsgAction::ACTION_LEAVE_MAP);
            player->send(&msg);
        }
    }
    mViewSet.clear();

    mViewSetMutex.unlock();

    // TODO map items
}

void
Entity :: addEntityToBCSet(const Entity& aEntity) const
{
    ASSERT(&aEntity != nullptr);
    ASSERT(&aEntity != this);

    mViewSetMutex.lock();

    if (mViewSet.end() == mViewSet.find(aEntity.getUID()))
    {
        mViewSet[aEntity.getUID()] = &aEntity;

        if (isPlayer())
        {
            Player* player = (Player*)this;
            aEntity.sendShow(*player);
        }
    }

    mViewSetMutex.unlock();
}

void
Entity :: removeEntityFromBCSet(const Entity& aEntity) const
{
    ASSERT(&aEntity != nullptr);
    ASSERT(&aEntity != this);

    mViewSetMutex.lock();

    map<uint32_t, const Entity*>::iterator it =
            mViewSet.find(aEntity.getUID());
    if (mViewSet.end() != it)
    {
        mViewSet.erase(it);

        if (isPlayer())
        {
            Player* player = (Player*)this;

            MsgAction msg(&aEntity, 0, MsgAction::ACTION_LEAVE_MAP);
            player->send(&msg);
        }
    }

    mViewSetMutex.unlock();
}

void
Entity :: broadcastRoomMsg(const Msg* aMsg, bool aIncludeSelf) const
{
    ASSERT(aMsg != nullptr);

    if (aIncludeSelf && isPlayer())
    {
        Player* player = (Player*)this;
        player->send(aMsg);
    }

    mViewSetMutex.lock();

    for (map<uint32_t, const Entity*>::const_iterator
            it = mViewSet.begin(), end = mViewSet.end();
         it != end; ++it)
    {
        const Entity& entity = *it->second;
        if (entity.isPlayer())
        {
            const Player* player = (const Player*)&entity;
            player->send(aMsg);
        }
    }

    mViewSetMutex.unlock();
}
