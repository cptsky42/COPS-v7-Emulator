/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_GENERATOR_H_
#define _COPS_V7_EMULATOR_GENERATOR_H_

#include "common.h"
#include "timer.h"

class GameMap;
class QSqlQuery;

/**
 * A generator is a monster's spawn handler.
 * It is totally based on the one created by TQ.
 */
class Generator
{
    friend class Monster;

public:
    /** Position of the SQL data in the result set. */
    enum SqlData
    {
        SQLDATA_ID = 0,
        SQLDATA_MAPID,
        SQLDATA_BOUND_X,
        SQLDATA_BOUND_Y,
        SQLDATA_BOUND_CX,
        SQLDATA_BOUND_CY,
        SQLDATA_MAXNPC,
        SQLDATA_REST_SECS,
        SQLDATA_MAX_PER_GEN,
        SQLDATA_MONSTER_TYPE
    };

public:
    /**
     * Create a new generator based on the result set of a query.
     *
     * @param[out]   aOutGenerator    the created object
     * @param[in]    aQuery           the result set of a query
     *
     * @retval ERROR_SUCCESS on success
     * @returns An error code otherwise
     */
    static err_t makeGenerator(Generator** aOutGenerator, const QSqlQuery& aQuery);

public:
    /**
     * Try to generate aAmount monsters.
     *
     * @param[in]    aAmount    the amount of monsters to generate
     *
     * @returns the number of generated monsters
     */
    uint32_t generate(uint32_t aAmount, bool aInitializing = false);

public:
    /* destructor */
    ~Generator() { }

private:
    /* constructor */
    Generator(uint32_t aId,
              GameMap& aMap,
              uint16_t aBoundX, uint16_t aBoundY, uint16_t aBoundCX, uint16_t aBoundCY,
              uint16_t aMaxNPC, uint16_t aRestSecs, uint16_t aMaxPerGen,
              uint32_t aMonsterType);

    /** Find an available position inside the valid range. */
    void findGenPos(uint16_t& aOutPosX, uint16_t& aOutPosY);

private:
    uint32_t mId; //!< the id of the generator
    uint32_t mMonsterType; //!< the generated monster type

    GameMap& mMap; //!< the map where the monsters are generated
    uint16_t mBoundX;
    uint16_t mBoundY;
    uint16_t mBoundCX;
    uint16_t mBoundCY;

    int32_t mGrid;
    uint16_t mRestSecs;
    uint16_t mMaxPerGen;

    uint32_t mAmount;
    uint32_t mGenAmount;
    uint32_t mCurGen;
    Timer mTimer;

    uint32_t mMaxNPC;
    uint32_t mIdxLastGen;
};

#endif // _COPS_V7_EMULATOR_GENERATOR_H_
