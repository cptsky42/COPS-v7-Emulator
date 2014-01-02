/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_DATE_H_
#define _COPS_V7_EMULATOR_MSG_DATE_H_

#include "common.h"
#include "msg.h"

/**
 * Msg sent to the client to update the local date.
 */
class MsgDate : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** Padding. */
        int32_t Padding;
        /** Years since 1900 */
        int32_t Year;
        /** Months since January */
        int32_t Month;
        /** Days since January 1 */
        int32_t DayOfYear;
        /** Day of the month */
        int32_t Day;
        /** Hours since midnight */
        int32_t Hour;
        /** Minutes after the hour */
        int32_t Minute;
        /** Seconds after the minute */
        int32_t Second;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgDate packet with current date & time.
     */
    MsgDate();

    /* destructor */
    virtual ~MsgDate();

private:
    /* internal filling of the packet */
    void create();

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_DATE_H_
