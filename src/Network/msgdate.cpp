/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgdate.h"
#include <time.h>

MsgDate :: MsgDate()
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create();
}

MsgDate :: ~MsgDate()
{

}

void
MsgDate :: create()
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_DATE;

    time_t timestamp = time(NULL);
    struct tm* tm = localtime(&timestamp);

    mInfo->Padding = 0;
    mInfo->Year = tm->tm_year;
    mInfo->Month = tm->tm_mon;
    mInfo->DayOfYear = tm->tm_yday;
    mInfo->Day = tm->tm_mday;
    mInfo->Hour = tm->tm_hour;
    mInfo->Minute = tm->tm_min;
    mInfo->Second = tm->tm_sec;
}

void
MsgDate :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Padding = bswap32(info->Padding);
    info->Year = bswap32(info->Year);
    info->Month = bswap32(info->Month);
    info->DayOfYear = bswap32(info->DayOfYear);
    info->Day = bswap32(info->Day);
    info->Hour = bswap32(info->Hour);
    info->Minute = bswap32(info->Minute);
    info->Second = bswap32(info->Second);
}
