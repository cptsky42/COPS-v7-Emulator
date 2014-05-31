/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgweather.h"

MsgWeather :: MsgWeather(uint32_t aType, uint32_t aIntensity, uint32_t aDirection,
                         uint32_t aColor)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aType, aIntensity, aDirection, aColor);
}

MsgWeather :: ~MsgWeather()
{

}

void
MsgWeather :: create(uint32_t aType, uint32_t aIntensity, uint32_t aDirection,
                     uint32_t aColor)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_WEATHER;

    mInfo->Type = aType;
    mInfo->Intensity = aIntensity;
    mInfo->Direction = aDirection;
    mInfo->Color = aColor;
}

void
MsgWeather :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Type = bswap32(info->Type);
    info->Intensity = bswap32(info->Intensity);
    info->Direction = bswap32(info->Direction);
    info->Color = bswap32(info->Color);
}

