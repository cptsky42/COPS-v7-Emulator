/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_WEATHER_H_
#define _COPS_V7_EMULATOR_MSG_WEATHER_H_

#include "common.h"
#include "msg.h"

/**
 * Msg sent to the client to update the weather of the map.
 */
class MsgWeather : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /**
         * Type of weather.
         *
         * Common values for Conquer Online are :
         *   1 - None
         *   2 - Rain
         *   3 - Snow
         *   4 - Rain Wind
         *   5 - Autumn Leaves
         *   7 - Cherry Blossom Petals
         *   8 - Cherry Blossom Petals Wind
         *   9 - Blowing Cotten
         *  10 - Atoms
         */
        uint32_t Type;
        /**
         * Intensity of the weather.
         * Value must be between 0 and 999.
         */
        uint32_t Intensity;
        /**
         * Direction of the weather.
         * Value must be between 0 and 359.
         */
        uint32_t Direction;
        /** Color of the weather. */
        uint32_t Color;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgWeather packet for the current map.
     *
     * @param[in]   aType       the type of the weather
     * @param[in]   aItensity   the intensity of the weather
     * @param[in]   aDirection  the direction of the weather
     * @param[in]   aColor      (optional) the color of the weather
     *                          Default value is 0.
     */
    MsgWeather(uint32_t aType, uint32_t aIntensity, uint32_t aDirection,
               uint32_t aColor = 0);

    /* destructor */
    virtual ~MsgWeather();

private:
    /* internal filling of the packet */
    void create(uint32_t aType, uint32_t aIntensity, uint32_t aDirection, uint32_t aColor);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_WEATHER_H_
