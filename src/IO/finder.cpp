/*
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
#include <sys/stat.h> // stat

#ifdef _WIN32
#include <Windows.h>
#include <io.h> // _open_osfhandle
#endif

using namespace std;

/* static */
bool
Finder :: fileExists(const char* aPath)
{
    ASSERT_ERR(aPath != nullptr && aPath[0] != '\0', ERROR_INVALID_PARAMETER);

    struct stat info;
    return stat(aPath, &info) == 0 ? true : false;
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
