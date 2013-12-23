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
    : mUID(aUID)
{
    mLook = 0;
    mMapId = 0;
    mPosX = 0;
    mPosY = 0;
    mDirection = 0;
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
    // TODO thread-safe
    for (map<uint32_t, const Entity*>::const_iterator
            it = mViewSet.begin(), end = mViewSet.end();
         it != end; ++it)
    {
        const Entity& entity = *it->second;
        entity.removeEntityFromBCSet(*this);
        removeEntityFromBCSet(entity);
    }
    mViewSet.clear();

    // TODO map items
}

void
Entity :: addEntityToBCSet(const Entity& aEntity) const
{
    ASSERT(&aEntity != nullptr);
    ASSERT(&aEntity != this);

    // TODO thread-safe
    if (mViewSet.end() == mViewSet.find(aEntity.getUID()))
    {
        mViewSet[aEntity.getUID()] = &aEntity;

        if (isPlayer())
        {
            Player* player = (Player*)this; // TODO: safe cast ?
            aEntity.sendShow(*player);
        }
    }
}

void
Entity :: removeEntityFromBCSet(const Entity& aEntity) const
{
    ASSERT(&aEntity != nullptr);
    ASSERT(&aEntity != this);

    if (isPlayer())
    {
        Player* player = (Player*)this; // TODO: safe cast ?

        MsgAction msg(&aEntity, 0, MsgAction::actionDisappear);
        player->send(&msg);
    }

    // TODO thread-safe
    map<uint32_t, const Entity*>::iterator it =
            mViewSet.find(aEntity.getUID());
    if (mViewSet.end() != it)
    {
        mViewSet.erase(it);
    }
}

void
Entity :: broadcastRoomMsg(Msg* aMsg, bool aIncludeSelf) const
{
    ASSERT(aMsg != nullptr);

    if (aIncludeSelf && isPlayer())
    {
        Player* player = (Player*)this; // TODO: safe cast ?
        player->send(aMsg);
    }

    // TODO thread safe
    for (map<uint32_t, const Entity*>::const_iterator
            it = mViewSet.begin(), end = mViewSet.end();
         it != end; ++it)
    {
        const Entity& entity = *it->second;
        if (entity.isPlayer())
        {
            const Player* player = (const Player*)&entity; // TODO: safe cast ?
            player->send(aMsg);
        }
    }
}

void
Entity :: broadcastRoomMsg(uint8_t* aBuf, size_t aLen, bool aIncludeSelf) const
{
    ASSERT(aBuf != nullptr);

    if (aIncludeSelf && isPlayer())
    {
        Player* player = (Player*)this; // TODO: safe cast ?
        player->send(aBuf, aLen);
    }

    // TODO thread safe
    for (map<uint32_t, const Entity*>::const_iterator
            it = mViewSet.begin(), end = mViewSet.end();
         it != end; ++it)
    {
        const Entity& entity = *it->second;
        if (entity.isPlayer())
        {
            const Player* player = (const Player*)&entity; // TODO: safe cast ?
            player->send(aBuf, aLen);
        }
    }
}
