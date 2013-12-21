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


#ifndef _COPS_V7_EMULATOR_FINDER_H_
#define _COPS_V7_EMULATOR_FINDER_H_

#include "common.h"
#include <stdio.h>

/**
  *
  */
class Finder
{
public:
    /**
      * Open the file pointed by the path with the "C" mode. The file
      * descriptor is returned.
      *
      * @param[out] aOutFile   a pointer to the object that will receive
      *                        the file descriptor
      * @param[in]  aPath      the path of the file to open
      * @param[in]  aMode      the "C" mode to use to open the file (e.g. w)
      *
      * @retval ERROR_SUCCESS on success
      * @retval ERROR_OPEN_FAILED on failure
      * @returns Error code otherwise
      */
    static err_t fileOpen(FILE** aOutFile,
                            const char* aPath, const char* aMode);

    /**
      * Get the file handle from the specified file descriptor.
      *
      * @param[in]  aFile   the file descriptor
      *
      * @returns The handle of the specified file
      */
    static int fileHandle(FILE* aFile);

    /**
      * Get an exclusive lock on the file pointed by the file descriptor.
      *
      * @param[in]  aFile   the file descriptor
      *
      * @retval ERROR_SUCCESS on success
      * @returns Error code otherwise
      */
    static err_t fileLock(FILE* aFile);

    /**
      * Release the exclusive lock on the file pointed by the file
      * descriptor.
      *
      * @param[in]  aFile   the file descriptor
      *
      * @retval ERROR_SUCCESS on success
      * @returns Error code otherwise
      */
    static err_t fileUnlock(FILE* aFile);

    /**
     * Return the current value of the position indicator of the stream.
     *
     * @param[in]  aFile   the file descriptor
     *
     * @retval -1 on failure
     * @returns The current value of the position indicator is returned on success
     */
    static int64_t fileTell(FILE* aFile);

    /**
     * Set the position indicator associated with the stream to a new position.
     *
     * @param[in]  aFile     the file descriptor
     * @param[in]  aOffset   the number of bytes to offset from origin
     * @param[in]  aWhence   the position used as reference for the offset
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_SEEK on failure
     * @returns Error code otherwise
     */
    static err_t fileSeek(FILE* aFile, int64_t aOffset, int aWhence);

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
