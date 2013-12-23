/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MAP_BASE_H_
#define _COPS_V7_EMULATOR_MAP_BASE_H_

/**
 * Represent a map's cell for a specific (X, Y) position.
 */
struct Cell
{
public:
    /** Determine wheter or not the cell is accessible. */
    bool Accessible;
    /** The altitude of the cell. */
    int16_t Altitude;
};

/**
 * Represent a map's passage to another one.
 */
struct Passage
{
public:
    /** The X position of the passage. */
    uint16_t PosX;
    /** The Y position of the passage. */
    uint16_t PosY;
    /** The index of the passage. */
    int32_t Index;
};

#endif // #define _COPS_V7_EMULATOR_MAP_BASE_H_
