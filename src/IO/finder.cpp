/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Taken from :
 * ****** BARLab - Open Source ******
 * Copyright (C) 2012 BARLab
 * Copyright (C) 2012 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "finder.h"

#include <sys/stat.h>
#include <string.h> // strerror
#include <algorithm> // max

#ifndef _WIN32
#include <sys/file.h> // flock BSD4.4
#endif

#ifdef _WIN32
#include <Windows.h>
#include <io.h> // _get_osfhandle
#endif

using namespace std;

/* static */
err_t
Finder :: fileOpen(FILE** aOutFile,
                   const char* aPath, const char* aMode)
{
    ASSERT_ERR(aOutFile != nullptr && *aOutFile == nullptr, ERROR_INVALID_PARAMETER);
    ASSERT_ERR(aPath != nullptr && aPath[0] != '\0', ERROR_INVALID_PARAMETER);
    ASSERT_ERR(aMode != nullptr && aMode[0] != '\0', ERROR_INVALID_PARAMETER);

    err_t err = ERROR_SUCCESS;

    #if defined(_WIN32)
    *aOutFile = fopen(aPath, aMode);
    if (*aOutFile == nullptr)
    {
        LOG(ERROR, "failed to open '%s' in '%s' : %s", aPath, aMode, strerror(errno));
        err = ERROR_OPEN_FAILED;
    }
    #elif defined(__APPLE__)
    *aOutFile = fopen(aPath, aMode);
    if (*aOutFile == nullptr)
    {
        LOG(ERROR, "failed to open '%s' in '%s' : %s", aPath, aMode, strerror(errno));
        err = ERROR_OPEN_FAILED;
    }
    #else
    *aOutFile = fopen64(aPath, aMode);
    if (*aOutFile == nullptr)
    {
        LOG(ERROR, "failed to open '%s' in '%s' : %s", aPath, aMode, strerror(errno));
        err = ERROR_OPEN_FAILED;
    }
    #endif

    return err;
}

/* static */
int
Finder :: fileHandle(FILE* aFile)
{
    ASSERT_ERR(aFile != nullptr, ERROR_INVALID_PARAMETER);

    int handle = 0;

    #if defined(_WIN32)
    handle = _fileno(aFile);
    #else
    handle = fileno(aFile);
    #endif

    return handle;
}

/* static */
err_t
Finder :: fileLock(FILE* aFile)
{
    ASSERT_ERR(aFile != nullptr, ERROR_INVALID_PARAMETER);

    err_t err = ERROR_SUCCESS;

    #if defined(_WIN32)
    if (!LockFile((HANDLE)_get_osfhandle(fileHandle(aFile)), 0, 0,
                    (uint32_t)(0x7FFFFFFFFFFFFFFFLL % 0x100000000LL),
                    (uint32_t)(0x7FFFFFFFFFFFFFFFLL / 0x100000000LL)))
    {
        err = ERROR_LOCK_VIOLATION;
    }
    #else
    if (flock(fileHandle(aFile), LOCK_EX) != 0)
    {
        err = ERROR_LOCK_VIOLATION;
    }
    #endif

    return err;
}

/* static */
err_t
Finder :: fileUnlock(FILE* aFile)
{
    ASSERT_ERR(aFile != nullptr, ERROR_INVALID_PARAMETER);

    err_t err = ERROR_SUCCESS;

    #if defined(_WIN32)
    if (!UnlockFile((HANDLE)_get_osfhandle(fileHandle(aFile)), 0, 0,
                  (uint32_t)(0x7FFFFFFFFFFFFFFFLL % 0x100000000LL),
                  (uint32_t)(0x7FFFFFFFFFFFFFFFLL / 0x100000000LL)))
    {
        err = ERROR_UNKNOWN;
    }
    #else
    if (flock(fileHandle(aFile), LOCK_UN) != 0)
    {
        err = ERROR_UNKNOWN;
    }
    #endif

    return err;
}

/* static */
int64_t
Finder :: fileTell(FILE* aFile)
{
    ASSERT_ERR(aFile != nullptr, ERROR_INVALID_PARAMETER);

    int64_t offset = 0;

    #if defined(_WIN32)
    offset = _ftelli64(aFile);
    #elif defined(__APPLE__)
    offset = ftell(aFile);
    #else
    offset = ftello64(aFile);
    #endif

    return offset;
}

/* static */
err_t
Finder :: fileSeek(FILE* aFile, int64_t aOffset, int aWhence)
{
    ASSERT_ERR(aFile != nullptr, ERROR_INVALID_PARAMETER);
    ASSERT_ERR(aWhence == SEEK_SET || aWhence == SEEK_CUR || aWhence == SEEK_END, ERROR_INVALID_PARAMETER);

    err_t err = ERROR_SUCCESS;

    #if defined(_WIN32)
    err = (_fseeki64(aFile, aOffset, aWhence) == 0 ? ERROR_SUCCESS : ERROR_SEEK);
    #elif defined(__APPLE__)
    err = (fseek(aFile, aOffset, aWhence) == 0 ? ERROR_SUCCESS : ERROR_SEEK);
    #else
    err = (fseeko64(aFile, aOffset, aWhence) == 0 ? ERROR_SUCCESS : ERROR_SEEK);
    #endif

    return err;
}

/* static */
bool
Finder :: fileExists(const char* aPath)
{
    ASSERT_ERR(aPath != nullptr && aPath[0] != '\0', ERROR_INVALID_PARAMETER);

    #if defined(_WIN32)
    struct _stat64 info;
    return _stat64(aPath, &info) == 0 ? true : false;
    #else
    struct stat info;
    return stat(aPath, &info) == 0 ? true : false;
    #endif
}

/* static */
FILE*
Finder :: getTempFile()
{
    FILE* file = nullptr;

    #if defined(_WIN32)
    wchar_t path[MAX_PATH];
    wchar_t name[MAX_PATH];

    HANDLE hFile = INVALID_HANDLE_VALUE;
    if (GetTempPathW(MAX_PATH, path) != 0)
    {
        if (GetTempFileNameW(path, L"CPT$", 0, name) != 0)
        {
            hFile = CreateFileW(name,
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,
                                NULL);
        }
    }

    if (hFile != INVALID_HANDLE_VALUE)
    {
        #ifdef _MSC_VER
        file = _fdopen(_open_osfhandle((intptr_t)hFile, 0), "rb+");
        #else
        file = fdopen(_open_osfhandle((intptr_t)hFile, 0), "rb+");
        #endif
    }
    #else
    file = tmpfile();
    #endif

    return file;
}
