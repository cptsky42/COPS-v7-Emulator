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

#include "finder.h"
#include <sys/stat.h> // stat

/* static */
bool
Finder :: fileExists(const char* aPath)
{
    assert(aPath != nullptr && aPath[0] != '\0');

    struct stat info;
    return stat(aPath, &info) == 0 ? true : false;
}
