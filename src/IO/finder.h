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


#ifndef _COPS_V7_EMULATOR_FINDER_H_
#define _COPS_V7_EMULATOR_FINDER_H_

#include "common.h"
#include <stdio.h>

class Finder
{
public:
    /**
     * Determine wheter or not a file exists at the specified path.
     *
     * @param[in]   aPath   the path of the file to check
     *
     * @retval TRUE if the file exists
     * @returns FALSE otherwise
     */
    static bool fileExists(const char* aPath);

    /**
     * Create a temp file that will be deleted on close.
     *
     * @returns A pointer to a FILE object that identifies the stream.
     */
    static FILE* getTempFile();
};

#endif /* _COPS_V7_EMULATOR_FINDER_H_ */
