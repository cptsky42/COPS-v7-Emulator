/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "mapdata.h"
#include "finder.h"
#include "binaryreader.h"
#include "lz4.h"
#include <stdlib.h>

using namespace std;

struct Cell;

/* static */
err_t
MapData :: load(MapData** aOutData, const char* aPath)
{
    ASSERT_ERR(aOutData != nullptr && *aOutData == nullptr, ERROR_INVALID_PARAMETER);
    ASSERT_ERR(aPath != nullptr && aPath[0] != '\0', ERROR_INVALID_PARAMETER);

    err_t err = ERROR_SUCCESS;

    MapData* data = new MapData();

    if (Finder::fileExists(aPath))
    {
        BinaryReader reader(aPath);

        DOIF(err, data->loadMapData(reader));

        reader.close();
    }
    else
    {
        LOG(ERROR, "Can't load the DMap file at %s as the file doesn't exist.",
            aPath);
        err = ERROR_FILE_NOT_FOUND;
    }

    if (IS_SUCCESS(err))
    {
        *aOutData = data;
        data = nullptr;
    }
    SAFE_DELETE(data);

    return err;
}

MapData :: MapData()
    : mWidth(0), mHeight(0), mCells(nullptr),
      mIsPacking(true), mPckData(nullptr), mPckLen(0)
{

}

MapData :: ~MapData()
{
    SAFE_DELETE_ARRAY(mCells);

    for (vector<Passage*>::iterator
            it = mPassages.begin(), end = mPassages.end();
         it != end; ++it)
    {
        Passage* passage = *it;
        SAFE_DELETE(passage);
    }

    free(mPckData);
    mPckData = nullptr;
}


err_t
MapData :: loadMapData(BinaryReader& aReader)
{
    ASSERT_ERR(&aReader != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    uint32_t version = 0, data = 0, width = 0, height = 0;
    DOIF(err, aReader.readUInt32(version));
    DOIF(err, aReader.readUInt32(data));

    // skipping puzzle file...
    DOIF(err, aReader.seek(sizeof(char) * MAX_PATH, SEEK_CUR));

    DOIF(err, aReader.readUInt32(width));
    DOIF(err, aReader.readUInt32(height));

    LOG(INFO, "TQ Digital DMap : v%u (data=%u) with [%u, %u] cells.",
        version, data, width, height);

    if (IS_SUCCESS(err))
    {
        if (width < UINT16_MAX && height < UINT16_MAX)
        {
            mWidth = (uint16_t)width;
            mHeight = (uint16_t)height;
        }
        else
        {
            LOG(ERROR, "Size overflow for the map.");
            err = ERROR_BAD_LENGTH;
        }
    }

    mCells = new Cell[mWidth * mHeight];
    for (uint16_t y = 0; ERROR_SUCCESS == err && y < mHeight; ++y)
    {
        uint32_t checksum = 0, tmp = 0;
        for (uint16_t x = 0; ERROR_SUCCESS == err && x < mWidth; ++x)
        {
            uint16_t mask = 0, terrain = 0;
            int16_t altitude = 0;

            DOIF(err, aReader.readUInt16(mask));
            DOIF(err, aReader.readUInt16(terrain));
            DOIF(err, aReader.readInt16(altitude));

            checksum += (uint32_t)((mask * (terrain + y + 1)) +
                                   ((altitude + 2) * (x + 1 + terrain)));

            Cell& cell = mCells[pos2idx(x, y)];
            cell.Accessible = mask != 1;
            cell.Altitude = altitude;
        }
        DOIF(err, aReader.readUInt32(tmp));

        if (checksum != tmp)
        {
            LOG(ERROR, "Invalid checksum for the block of cells for the position y=%u", y);
            err = ERROR_BAD_CHECKSUM;
        }
    }

    DOIF(err, loadPassageData(aReader));
//    if (version == 1003)
//        DOIF(err, loadRegionData(aReader));
    DOIF(err, loadLayerData(aReader));

    //The rest are LAYER_SCENE, but useless for a server. I'll not implement the rest as it would only
    //slow down the loading.

    return err;
}

err_t
MapData :: loadPassageData(BinaryReader& aReader)
{
    ASSERT_ERR(&aReader != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    int32_t count = 0;
    DOIF(err, aReader.readInt32(count));
    mPassages.reserve(count);

    LOG(INFO, "Found %d passages for the map.", count);

    for (int32_t i = 0; ERROR_SUCCESS == err && i < count; ++i)
    {
        Passage* passage = new Passage();
        uint32_t posX = 0, posY = 0;

        DOIF(err, aReader.readUInt32(posX));
        DOIF(err, aReader.readUInt32(posY));
        DOIF(err, aReader.readInt32(passage->Index));

        if (IS_SUCCESS(err))
        {
            if (posX < UINT16_MAX && posX < UINT16_MAX)
            {
                passage->PosX = (uint16_t)posX;
                passage->PosY = (uint16_t)posY;
            }
            else
            {
                LOG(ERROR, "Size overflow for the position of the passage.");
                err = ERROR_BAD_LENGTH;
            }
        }

        if (IS_SUCCESS(err))
        {
            LOG(VRB, "Added passage %d at (%u, %u).",
                passage->Index, passage->PosX, passage->PosY);

            mPassages.push_back(passage);
            passage = nullptr;
        }
        SAFE_DELETE(passage);
    }

    return err;
}

err_t
MapData :: loadRegionData(BinaryReader& aReader)
{
    ASSERT_ERR(&aReader != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    int32_t count = 0;
    DOIF(err, aReader.readInt32(count));

    LOG(INFO, "Found %d regions for the map.", count);

    if (IS_SUCCESS(err))
    {
        LOG(WARN, "Regions are not supported yet.");
    }

    return err;
}

err_t
MapData :: loadLayerData(BinaryReader& aReader)
{
    ASSERT_ERR(&aReader != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    int32_t count = 0;
    DOIF(err, aReader.readInt32(count));

    LOG(INFO, "Found %d layers for the map.", count);

    for (int32_t i = 0; ERROR_SUCCESS == err && i < count; ++i)
    {
        int32_t type = MAP_NONE;
        DOIF(err, aReader.readInt32(type));

        switch (type)
        {
        case MAP_COVER: // 2DMapCoverObj
            {
                //Do nothing with it...
                LOG(VRB, "Found a 2D map cover object layer. Skipping.");
                DOIF(err, aReader.seek(416, SEEK_CUR));
                break;
            }
        case MAP_TERRAIN: // 2DMapTerrainObj
            {
                // A class could be good... But useless for a server implementation.

                char fileName[MAX_PATH];
                uint32_t startX = 0, startY = 0;
                int32_t count = 0;

                DOIF(err, aReader.read(fileName, sizeof(fileName)));
                DOIF(err, aReader.readUInt32(startX));
                DOIF(err, aReader.readUInt32(startY));

                #ifndef _WIN32 // convert path separator...
                if (IS_SUCCESS(err))
                {
                    for (size_t x = 0, len = strlen(fileName); x < len; ++x)
                    {
                        if (fileName[x] == '\\')
                            fileName[x] = '/';
                    }
                }
                #endif // _WIN32

                LOG(VRB, "Found a 2D map terrain object at (%u, %u). Loading scene file '%s'",
                    startX, startY, fileName);

                if (Finder::fileExists(fileName))
                {
                    BinaryReader reader(fileName);

                    DOIF(err, reader.readInt32(count));
                    LOG(VRB, "Found %d parts.", count);

                    // the server only need the new cells info, so it will be merged
                    // and the objects will be deleted...
                    // it is useless to parse most of the parts info...
                    for (int32_t i = 0; ERROR_SUCCESS == err && i < count; ++i) // all parts
                    {
                        uint32_t width = 0, height = 0;
                        uint32_t sceneOffsetX = 0, sceneOffsetY;
                        DOIF(err, reader.seek(256, SEEK_CUR)); // AniFile
                        DOIF(err, reader.seek(64, SEEK_CUR)); // AniTitle
                        DOIF(err, reader.seek(sizeof(uint32_t), SEEK_CUR)); // PosOffset(X)
                        DOIF(err, reader.seek(sizeof(uint32_t), SEEK_CUR)); // PosOffset(Y)
                        DOIF(err, reader.seek(sizeof(uint32_t), SEEK_CUR)); // AniInterval
                        DOIF(err, reader.readUInt32(width));
                        DOIF(err, reader.readUInt32(height));
                        DOIF(err, reader.seek(sizeof(int32_t), SEEK_CUR)); // Thick
                        DOIF(err, reader.readUInt32(sceneOffsetX));
                        DOIF(err, reader.readUInt32(sceneOffsetY));
                        DOIF(err, reader.seek(sizeof(int32_t), SEEK_CUR)); // Height

                        for (uint32_t y = 0; ERROR_SUCCESS == err && y < height; ++y)
                        {
                            for (uint32_t x = 0; ERROR_SUCCESS == err && x < width; ++x)
                            {
                                uint32_t mask = 0, terrain = 0;
                                int32_t altitude = 0;

                                DOIF(err, reader.readUInt32(mask));
                                DOIF(err, reader.readUInt32(terrain));
                                DOIF(err, reader.readInt32(altitude));

                                size_t posX = ((startX + sceneOffsetX) + x) - width;
                                size_t posY = ((startY + sceneOffsetY) + y) - height;
                                if (posX < UINT16_MAX && posX < UINT16_MAX)
                                {
                                    Cell& cell = mCells[pos2idx((uint16_t)posX, (uint16_t)posY)];
                                    cell.Accessible = mask != false;
                                    cell.Altitude = altitude;
                                }
                                else
                                {
                                    LOG(ERROR, "Size overflow for the position of the scene part.");
                                    err = ERROR_BAD_LENGTH;
                                }
                            }
                        }
                    }

                    reader.close();
                }
                else
                {
                    LOG(ERROR, "Can't load the scene file at '%s'' as the file doesn't exist.",
                        fileName);
                    err = ERROR_FILE_NOT_FOUND;
                }

                break;
            }
        case MAP_SOUND: // MapSound
            {
                //Do nothing with it...
                LOG(VRB, "Found a map sound layer. Skipping.");
                DOIF(err, aReader.seek(276, SEEK_CUR));
                break;
            }
        case MAP_3DEFFECT: // 3DMapEffect
            {
                //Do nothing with it...
                LOG(VRB, "Found a 3D map effect layer. Skipping.");
                DOIF(err, aReader.seek(72, SEEK_CUR));
                break;
            }
        case MAP_3DEFFECTNEW: // 3DMapEffectNew
            {
                //Do nothing with it...
                LOG(VRB, "Found a 3D map effect (new) layer. Skipping.");
                DOIF(err, aReader.seek(96, SEEK_CUR));
                break;
            }
        default:
            {
                LOG(VRB, "Found a layer of type %d at offset %lld, which is unknown.",
                    type, aReader.tell());
                err = ERROR_BAD_FORMAT;
                break;
            }
        }
    }

    return err;
}

err_t
MapData :: pack(void* aCaller)
{
    err_t err = ERROR_SUCCESS;

    mPckMutex.lock();

    if (aCaller != nullptr)
        mRefs.erase(aCaller);

    if (mIsPacking && mPckData == nullptr && mRefs.size() == 0)
    {
        ASSERT(mCells != nullptr);

        size_t len = mWidth * mHeight * (sizeof(uint8_t) + sizeof(int16_t));
        uint8_t* buf = new uint8_t[len];
        uint8_t* ptr = buf;

        for (uint16_t y = 0; ERROR_SUCCESS == err && y < mHeight; ++y)
        {
            for (uint16_t x = 0; ERROR_SUCCESS == err && x < mWidth; ++x)
            {
                const Cell& cell = mCells[pos2idx(x, y)];

                *(ptr++) = cell.Accessible ? true : false;
                *(ptr++) = cell.Altitude & INT16_C(0xFF);
                *(ptr++) = (cell.Altitude >> 8) & INT16_C(0xFF);
            }
        }

        // LZ4 might not be able to compress... alloc worst case...
        mPckData = (uint8_t*)malloc(LZ4_compressBound(len));

        int newlen = LZ4_compress((char*)buf, (char*)mPckData, len);

        mPckData = (uint8_t*)realloc(mPckData, newlen);
        mPckLen = newlen;

        SAFE_DELETE_ARRAY(buf);
        if (IS_SUCCESS(err))
        {
            SAFE_DELETE_ARRAY(mCells);
        }
        else
        {
            free(mPckData);
            mPckData = nullptr;
        }
    }

    mPckMutex.unlock();
    return err;
}

err_t
MapData :: unpack(void* aCaller)
{
    err_t err = ERROR_SUCCESS;

    mPckMutex.lock();

    if (aCaller != nullptr)
        mRefs.insert(aCaller);

    if (mIsPacking && mPckData != nullptr)
    {
        ASSERT(mCells == nullptr);

        mCells = new Cell[mWidth * mHeight];

        size_t len = mWidth * mHeight * (sizeof(uint8_t) + sizeof(int16_t));
        uint8_t* buf = new uint8_t[len];
        uint8_t* ptr = buf;

        LZ4_decompress_fast((char*)mPckData, (char*)buf, len);

        for (uint16_t y = 0; ERROR_SUCCESS == err && y < mHeight; ++y)
        {
            for (uint16_t x = 0; ERROR_SUCCESS == err && x < mWidth; ++x)
            {
                Cell& cell = mCells[pos2idx(x, y)];
                cell.Accessible = *(ptr++) != false;

                uint8_t low = *(ptr++);
                uint8_t high = *(ptr++);
                cell.Altitude = (int16_t)((high << 8) | low);
            }
        }

        SAFE_DELETE_ARRAY(buf);
        if (IS_SUCCESS(err))
        {
            free(mPckData);
            mPckData = nullptr;
        }
        else
        {
            SAFE_DELETE_ARRAY(mCells);
        }
    }

    mPckMutex.unlock();
    return err;
}

int
MapData :: getPassage(uint16_t aPosX, uint16_t aPosY) const
{
    int passageId = -1;

    for (vector<Passage*>::const_iterator
            it = mPassages.begin(), end = mPassages.end();
         it != end; ++it)
    {
        const Passage& passage = **it;
        if (passage.PosX == aPosX && passage.PosY == aPosY)
        {
            passageId = passage.Index;
            break;
        }
    }

    return passageId;
}
