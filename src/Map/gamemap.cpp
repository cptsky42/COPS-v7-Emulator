/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "gamemap.h"
#include "entity.h"
#include "player.h"
#include "msgmapinfo.h"
#include <vector>

using namespace std;

GameMap :: GameMap(uint32_t aUID, Info** aInfo, MapData& aData)
    : mUID(aUID), mInfo(*aInfo), mData(aData), mPlayerCount(0)
{
    ASSERT(aInfo != nullptr && *aInfo != nullptr);
    *aInfo = nullptr;
}

GameMap :: ~GameMap()
{
    SAFE_DELETE(mInfo);
}

bool
GameMap :: getFloorAccess(uint16_t aPosX, uint16_t aPosY) const
{
    const Cell& cell = mData.getCell(aPosX, aPosY);
    return cell.Accessible;
}

int16_t
GameMap :: getFloorAlt(uint16_t aPosX, uint16_t aPosY) const
{
    const Cell& cell = mData.getCell(aPosX, aPosY);
    return cell.Altitude;
}

void
GameMap :: sendMapInfo(const Player& aPlayer) const
{
    ASSERT(&aPlayer != nullptr);

    mEntitiesMutex.lock();

    // the player must be on the map...
    if (mEntities.end() != mEntities.find(aPlayer.getUID()))
    {
        MsgMapInfo msg(*this);
        aPlayer.send(&msg);
    }

    mEntitiesMutex.unlock();
}

void
GameMap :: sendBlockInfo(const Player& aPlayer) const
{
    ASSERT(&aPlayer != nullptr);

    mEntitiesMutex.lock();

    // the player must be on the map...
    if (mEntities.end() != mEntities.find(aPlayer.getUID()))
    {
        for (map<uint32_t, Entity*>::const_iterator
                it = mEntities.begin(), end = mEntities.end();
             it != end; ++it)
        {
            const Entity& entity = *it->second;

            if (entity.getUID() == aPlayer.getUID())
                continue;

            if (distance(aPlayer.getPosX(), aPlayer.getPosY(),
                         entity.getPosX(), entity.getPosY()) < Entity::CELLS_PER_VIEW)
            {
                // send the entity spawn...
                entity.sendShow(aPlayer);
            }
        }
    }

    mEntitiesMutex.unlock();
}

void
GameMap :: updateBroadcastSet(const Entity& aEntity) const
{
    ASSERT(&aEntity != nullptr);

    mEntitiesMutex.lock();

    // the entity must be on the map...
    if (mEntities.end() != mEntities.find(aEntity.getUID()))
    {
        // TODO: items
        for (map<uint32_t, Entity*>::const_iterator
                it = mEntities.begin(), end = mEntities.end();
             it != end; ++it)
        {
            const Entity& entity = *it->second;

            if (entity.getUID() == aEntity.getUID())
                continue;

            if (distance(aEntity.getPosX(), aEntity.getPosY(),
                         entity.getPosX(), entity.getPosY()) <= Entity::CELLS_PER_VIEW)
            {
                // add the entity to the set...
                aEntity.addEntityToBCSet(entity);
                entity.addEntityToBCSet(aEntity);
            }
            else
            {
                // try to remove the entity from the set...
                aEntity.removeEntityFromBCSet(entity);
                entity.removeEntityFromBCSet(aEntity);
            }
        }
    }

    mEntitiesMutex.unlock();
}

void
GameMap :: enterRoom(Entity& aEntity)
{
    mEntitiesMutex.lock();

    map<uint32_t, Entity*>::iterator it = mEntities.find(aEntity.getUID());
    if (mEntities.end() == it)
    {
        // activate the map...
        if (aEntity.isPlayer())
        {
            if (mPlayerCount == 0)
                mData.unpack(this);
            ++mPlayerCount;
        }

        mEntities[aEntity.getUID()] = &aEntity;
    }

    mEntitiesMutex.unlock();
}

void
GameMap :: leaveRoom(Entity& aEntity)
{
    mEntitiesMutex.lock();

    map<uint32_t, Entity*>::iterator it = mEntities.find(aEntity.getUID());
    if (mEntities.end() != it)
    {
        mEntities.erase(it);

        // desactivate the map...
        if (aEntity.isPlayer())
        {
            --mPlayerCount;
            if (mPlayerCount == 0)
                mData.pack(this);
        }
    }

    mEntitiesMutex.unlock();
}
