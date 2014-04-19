/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MS_TIMER_H_
#define _COPS_V7_EMULATOR_MS_TIMER_H_

#include "common.h"
#include <time.h>

/**
 * Timer (in milliseconds) to determine whether an interval is passed...
 */
class MSTimer
{
public:
    MSTimer(uint32_t aInterval = 0) : mLastClock(0), mInterval(aInterval) { }

    /* destructor */
    ~MSTimer() { }

public:
    /** Start the timer. */
    void start(uint32_t aInterval) { mInterval = aInterval; update(); }

    /** Update the last clock for the start of the timer. */
    void update() { mLastClock = (clock() * CLOCK_MULTIPLIER); }

    /** Clear the timer. */
    void clear() { mLastClock = 0; mInterval = 0; }

    /**
     * Determine if the interval is passed. If it passed, the timer will be
     * updated for the next one.
     *
     * @retval TRUE if the interval is passed
     * @returns FALSE otherwise
     */
    bool toNextTime() { if (isTimeOut()) return update(), true; else return false; }

    /**
     * Determine if the interval is passed. If it passed, the timer cleared.
     *
     * @retval TRUE if the interval is passed
     * @returns FALSE otherwise
     */
    bool timeOver() { if (isActive() && isTimeOut()) return clear(), true; return false; }

public:
    /** Set a new interval. */
    void setInterval(uint32_t aInterval) { mInterval = aInterval; }

public:
    /** Determine whether or not the timer is active. */
    bool isActive() { return mLastClock != 0; }
    /** Determine whether or not the interval passed. */
    bool isTimeOut() { return (clock() * CLOCK_MULTIPLIER) >= mLastClock + mInterval; }

private:
    /* Multiplier to get the clock time in milliseconds instead of seconds. */
    static const double CLOCK_MULTIPLIER; // see basefunc.cpp for the definition...

private:
    clock_t mLastClock; //!< the last clock
    uint32_t mInterval; //!< the interval of the event
};

#endif // _COPS_V7_EMULATOR_MS_TIMER_H_
