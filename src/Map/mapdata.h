/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MAP_DATA_H_
#define _COPS_V7_EMULATOR_MAP_DATA_H_

#include "common.h"
#include "mapbase.h"
#include <vector>
#include <set>
#include <QMutex>

class BinaryReader;

/**
 * Map data (DMap)
 */
class MapData
{
    // !!! class has pointer data members !!!
    PROHIBIT_COPY(MapData);

public:
    /**
     * Load a DMap as a MapData object containing cells and passages.
     *
     * @param[in,out]   aOutData     a reference to the outputed MapData
     * @param[in]       aPath        the path of the DMap file
     *
     * @retval ERROR_SUCCESS on success
     * @returns an error code otherwise
     */
    static err_t load(MapData** aOutData, const char* aPath);

public:
    /* destructor */
    ~MapData();

public:
    /**
     * Compress the map data in memory.
     *
     * @retval ERROR_SUCCESS on success
     * @returns An error code otherwise
     */
    err_t pack(void* aCaller = nullptr);

    /**
     * Decompress the map data in memory.
     *
     * @retval ERROR_SUCCESS on success
     * @returns An error code otherwise
     */
    err_t unpack(void* aCaller = nullptr);

public:
    /** Get the width of the map. */
    uint16_t getWidth() const { return mWidth; }
    /** Get the height of the map. */
    uint16_t getHeight() const { return mHeight; }

    /** Get the cell information of a specific position. */
    const Cell& getCell(uint16_t aPosX, uint16_t aPosY) const
    { return mCells[pos2idx(aPosX, aPosY)]; }

    /** Get the passage ID of the given coords. */
    int getPassage(uint16_t aPosX, uint16_t aPosY) const;

private:
    #ifndef _WIN32
    static const int32_t MAX_PATH = 260;
    #endif

    static const int32_t MAP_NONE = 0;
    static const int32_t MAP_TERRAIN = 1;
    static const int32_t MAP_TERRAIN_PART = 2;
    static const int32_t MAP_SCENE = 3;
    static const int32_t MAP_COVER = 4;
    static const int32_t MAP_ROLE = 5;
    static const int32_t MAP_HERO = 6;
    static const int32_t MAP_PLAYER = 7;
    static const int32_t MAP_PUZZLE = 8;
    static const int32_t MAP_3DSIMPLE = 9;
    static const int32_t MAP_3DEFFECT = 10;
    static const int32_t MAP_2DITEM = 11;
    static const int32_t MAP_3DNPC = 12;
    static const int32_t MAP_3DOBJ = 13;
    static const int32_t MAP_3DTRACE = 14;
    static const int32_t MAP_SOUND = 15;
    static const int32_t MAP_2DREGION = 16;
    static const int32_t MAP_3DMAGICMAPITEM = 17;
    static const int32_t MAP_3DITEM = 18;
    static const int32_t MAP_3DEFFECTNEW = 19;

private:
    /* constructor */
    MapData();

private:
    /** Load the map data */
    err_t loadMapData(BinaryReader& aReader);
    /** Load all passages data */
    err_t loadPassageData(BinaryReader& aReader);
    /** Load all regions data */
    err_t loadRegionData(BinaryReader& aReader);
    /** Load all layers data */
    err_t loadLayerData(BinaryReader& aReader);

private:
    /** Convert a position to an index. */
    inline size_t pos2idx(uint16_t aPosX, uint16_t aPosY) const { return (aPosX + aPosY * mWidth); }
    /** Convert an index to a x-coord. */
    inline size_t idx2x(size_t aIdx) const { return (aIdx % mHeight); }
    /** Convert a position to a y-coord. */
    inline size_t idx2y(size_t aIdx) const { return (aIdx / mHeight); }

public:
    /** MUST NOT BE USED ! Suspend the automatic packing ! */
    void suspendPacking() { mIsPacking = false; }
    /** MUST NOT BE USED ! Resume the automatic packing ! */
    void resumePacking() { mIsPacking = true; }

private:
    uint16_t mWidth;  //!< the width (number of cell) of the map
    uint16_t mHeight; //!< the height (number of cell) of the map

    Cell* mCells; //!< all the cells of the map
    std::vector<Passage*> mPassages; //!< all the passages of the map

    bool mIsPacking; //!< determine whether the MapData must handle the automatic packing
    std::set<void*> mRefs; //!< the pointers of active map using the data
    uint8_t* mPckData; //!< the packed data of the map (cells)
    size_t mPckLen; //!< the size of the packed data
    QMutex mPckMutex; //!< the mutex to pack / unpack the data...
};

#endif // _COPS_V7_EMULATOR_MAP_DATA_H_
