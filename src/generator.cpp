/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "generator.h"
#include "basefunc.h"
#include "mapmanager.h"
#include "gamemap.h"
#include "world.h"
#include "monster.h"
#include <QSqlQuery>
#include <QVariant>

/* static */
err_t
Generator :: makeGenerator(Generator** aOutGenerator, const QSqlQuery& aQuery)
{
    ASSERT_ERR(aOutGenerator != nullptr && *aOutGenerator == nullptr, ERROR_INVALID_POINTER);
    ASSERT_ERR(&aQuery != nullptr, ERROR_INVALID_REFERENCE);

    static MapManager& mgr = MapManager::getInstance(); // singleton...

    err_t err = ERROR_SUCCESS;

    uint32_t id = (uint32_t)aQuery.value(SQLDATA_ID).toUInt();
    uint32_t mapId = (uint32_t)aQuery.value(SQLDATA_MAPID).toUInt();
    GameMap* map = mgr.getMap(mapId);

    if (map != nullptr)
    {
        Generator* generator = new Generator(id, *map,
                                             (uint16_t)aQuery.value(SQLDATA_BOUND_X).toUInt(),
                                             (uint16_t)aQuery.value(SQLDATA_BOUND_Y).toUInt(),
                                             (uint16_t)aQuery.value(SQLDATA_BOUND_CX).toUInt(),
                                             (uint16_t)aQuery.value(SQLDATA_BOUND_CY).toUInt(),
                                             (uint16_t)aQuery.value(SQLDATA_MAXNPC).toUInt(),
                                             (uint16_t)aQuery.value(SQLDATA_REST_SECS).toUInt(),
                                             (uint16_t)aQuery.value(SQLDATA_MAX_PER_GEN).toUInt(),
                                             (uint32_t)aQuery.value(SQLDATA_MONSTER_TYPE).toUInt());
        *aOutGenerator = generator;
        generator = nullptr;

        SAFE_DELETE(generator);
    }
    else
    {
        LOG(WARN, "Could not find the map %u for the generator %u.",
            mapId, id);
        err = ERROR_NOT_FOUND;
    }

    return err;
}

Generator :: Generator(uint32_t aId,
                       GameMap& aMap,
                       uint16_t aBoundX, uint16_t aBoundY, uint16_t aBoundCX, uint16_t aBoundCY,
                       uint16_t aMaxNPC, uint16_t aRestSecs, uint16_t aMaxPerGen,
                       uint32_t aMonsterType)
    : mId(aId), mMonsterType(aMonsterType),
      mMap(aMap),
      mBoundX(aBoundX), mBoundY(aBoundY), mBoundCX(aBoundCX), mBoundCY(aBoundCY),
      mGrid(aMaxNPC), mRestSecs(aRestSecs), mMaxPerGen(aMaxPerGen),
      mAmount(0), mGenAmount(0), mCurGen(0),
      mMaxNPC(0), mIdxLastGen(0)
{
    mTimer.setInterval(mRestSecs + random(0, mRestSecs));

    if (mGrid < 1)
        mMaxNPC = 1;
    else
        mMaxNPC = (mBoundCX / mGrid) * (mBoundCY / mGrid);

    if (mMaxNPC < 1)
        mMaxNPC = 1;

    mIdxLastGen = random(0, mMaxNPC);
}

void
Generator :: findGenPos(uint16_t& aOutPosX, uint16_t& aOutPosY)
{
    ASSERT(&aOutPosX != nullptr && &aOutPosY != nullptr);

    aOutPosX = mBoundX;
    aOutPosY = mBoundY;
    if (mMaxNPC <= 1)
    {
        aOutPosX += random(0, mBoundCX);
        aOutPosY += random(0, mBoundCY);
    }
    else
    {
        ++mIdxLastGen;
        if (mIdxLastGen >= mMaxNPC)
            mIdxLastGen = 0;

        int32_t gridX = mBoundCX / mGrid;
        if (gridX < 0)
            gridX = 1;

        aOutPosX += mGrid * (mIdxLastGen % gridX) + random(0, mGrid);
        aOutPosY += mGrid * (mIdxLastGen / gridX) + random(0, mGrid);
    }
}

uint32_t
Generator :: generate(uint32_t aAmount, bool aInitializing /* = false */)
{
    static World& world = World::getInstance(); // singleton...
    const uint32_t RANDOM_GENERATOR_SECS = 600;

    if (!aInitializing && !mMap.isActive())
        return 0;


    if (mGenAmount >= mMaxNPC)
        return 0;

    if (mTimer.toNextTime())
    {
        if (mRestSecs >= RANDOM_GENERATOR_SECS)
            mTimer.start(mRestSecs + random(0, mRestSecs));

        mCurGen = mMaxPerGen;
    }

    if (mCurGen <= 0)
        return 0;

    // must access map data... will do a lot of I/O if no player is on the map
    // but acceptable as it will only happens at startup
    mMap.mData.unpack(this);

    uint32_t generated = 0;
    for (uint32_t i = 0; i < aAmount; ++i)
    {
        uint16_t posX = 0, posY = 0;
        findGenPos(posX, posY);

        if (!mMap.isStandEnable(posX, posY))
        {
            posX = mBoundX + random(0, mBoundCX);
            posY = mBoundY + random(0, mBoundCY);

            if (!mMap.isStandEnable(posX, posY))
                continue;
        }

        Monster* monster = world.generateMonster(mMonsterType, this);
        if (monster != nullptr)
        {
            monster->setMapId(mMap.getUID());
            monster->setPosition(posX, posY);

            // add the monster to the map
            mMap.enterRoom(*monster);

            // TODO broadcast spawn ?
        }

        --mCurGen;
        ++mGenAmount;
        ++generated;
        if (mCurGen <= 0)
            break;
    }

    // release the resources
    mMap.mData.pack(this);

    return generated;
}
