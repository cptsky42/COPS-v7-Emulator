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
    : mUID(aUID), mInfo(*aInfo), mData(aData)
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

    // the player must be on the map...
    if (mEntities.end() != mEntities.find(aPlayer.getUID()))
    {
        MsgMapInfo msg(*this);
        aPlayer.send(&msg);
    }
}

void
GameMap :: sendBlockInfo(const Player& aPlayer) const
{
    ASSERT(&aPlayer != nullptr);

    // the player must be on the map...
    if (mEntities.end() != mEntities.find(aPlayer.getUID()))
    {
        // TODO: thread-safe
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
}

void
GameMap :: updateBroadcastSet(const Entity& aEntity) const
{
    ASSERT(&aEntity != nullptr);

    // the entity must be on the map...
    if (mEntities.end() != mEntities.find(aEntity.getUID()))
    {
        // TODO: items
        // TODO: thread-safe
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

            // TODO remove others
        }
    }
}

void
GameMap :: enterRoom(Entity& aEntity)
{
    // activate the map...
    if (mEntities.size() == 0)
    {
        mData.unpack();
    }

    //TODO
    mEntities[aEntity.getUID()] = &aEntity;
}

void
GameMap :: leaveRoom(Entity& aEntity)
{
    // TODO

    // desactivate the map...
    if (mEntities.size() == 0)
    {
        mData.pack();
    }
}
