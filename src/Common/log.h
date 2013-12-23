/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_LOG_H_
#define _COPS_V7_EMULATOR_LOG_H_

#include "env.h"
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class Logger;

#define LOG_(LEVEL, ...) \
    if (Logger::willLog(LEVEL)) \
        Logger::log(LEVEL, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define LOG(LEVEL, ...) LOG_(LOG_LEVEL_ ## LEVEL, __VA_ARGS__)

/** Level of logging. */
enum LogLevel
{
    /** Verbose */
    LOG_LEVEL_VRB = 0,
    /** Debug */
    LOG_LEVEL_DBG = 1,
    /** Information */
    LOG_LEVEL_INFO = 2,
    /** Warning */
    LOG_LEVEL_WARN = 3,
    /** Error */
    LOG_LEVEL_ERROR = 4
};

/**
 * Optimized logger.
 */
class Logger : public Environment::Global, private QThread
{
public:
    static err_t init(const char* aDestFolder, const char* aFile);

    static bool willLog(LogLevel aLevel);

    static void log(LogLevel aLevel,
                    const char* aFile, const char* aFunction, unsigned int aLine,
                    const char* aFormat, ...);

public:
    /* destructor */
    ~Logger();

private:
    /* time / string */
    typedef std::pair<tm, char*> LogData;

private:
    void log(LogLevel aLevel,
             const char* aFile, const char* aFunction, unsigned int aLine,
             const char* aFormat, va_list& aArgs);

    virtual void run();

    /* constructor */
    Logger();

private:
    static inline tm getCurrentTime()
    {
        time_t now = time(NULL);
        return *gmtime(&now);
    }

    static Logger& getInstance();

private:
    static Logger* sInstance;

private:
    std::string mDestFolder;
    std::string mFile;

    FILE* mStream;
    std::vector<LogData*> mQueue;

    QMutex mMutex;
    QWaitCondition mQueueNotEmpty;
    bool mHasToShutdown;
};

#endif // _COPS_V7_EMULATOR_LOG_H_
