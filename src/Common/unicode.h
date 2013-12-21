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

#ifndef _COPS_V7_EMULATOR_UNICODE_H_
#define _COPS_V7_EMULATOR_UNICODE_H_

#include "types.h"
#include "err.h"

#ifdef _WIN32
#include <windows.h>
#include <wchar.h>
#endif // _WIN32

#ifdef _WIN32
/**
  * Convert a null-terminated wide string to a null-terminated UTF-8 string.
  * The function will silently drops illegal code points.
  *
  * @param[out]    aOutStr  a pointer to the buffer that will receive the
  *                         encoded string
  * @param[in,out] aLen     the length of the output buffer
  *                         on success, the parameter is set to the used length
  *                         on failure, the parameter is set to the required length
  * @param[in]     aWStr    a null-terminated wide string
  *
  * @retval ERROR_SUCCESS on success
  * @retval ERROR_BUFFER_OVERFLOW if the buffer is too small
  * @returns Error code otherwise
  */
inline err_t wcToUtf8(char* aOutStr, size_t& aLen, const wchar_t* aStr)
{
    ASSERT(aOutStr != nullptr);
    ASSERT(aStr != nullptr);
    ASSERT(aLen <= INT_MAX);

    err_t err = ERROR_SUCCESS;

    int len = WideCharToMultiByte(CP_UTF8, 0, aStr, -1, aOutStr, (int)aLen, NULL, NULL);
    if (len == 0)
    {
        // failed to convert the string, will return the required length
        aLen = WideCharToMultiByte(CP_UTF8, 0, aStr, -1, NULL, 0, NULL, NULL);
        err = ERROR_BUFFER_OVERFLOW;
    }
    else
    {
        // converted the string, return the used length
        aLen = len;
    }

    return err;
}

/**
  * Convert a null-terminated UTF-8 string to a null-terminated wide string.
  *
  * @param[out]    aOutStr  a pointer to the buffer that will receive the
  *                         encoded string
  * @param[in,out] aLen     the length of the output buffer
  *                         on success, the parameter is set to the used length
  *                         on failure, the parameter is set to the required length
  * @param[in]     aWStr    a null-terminated wide string
  *
  * @retval ERROR_SUCCESS on success
  * @retval ERROR_BUFFER_OVERFLOW if the buffer is too small
  * @returns Error code otherwise
  */
inline err_t utf8ToWc(wchar_t* aOutStr, size_t& aLen, const char* aStr)
{
    ASSERT(aOutStr != nullptr);
    ASSERT(aStr != nullptr);
    ASSERT(aLen <= INT_MAX);

    err_t err = ERROR_SUCCESS;

    int len = MultiByteToWideChar(CP_UTF8, 0, aStr, -1, aOutStr, (int)aLen);
    if (len == 0)
    {
        // failed to convert the string, will return the required length
        aLen = MultiByteToWideChar(CP_UTF8, 0, aStr, -1, NULL, 0);
        err = ERROR_BUFFER_OVERFLOW;
    }
    else
    {
        // converted the string, return the used length
        aLen = len;
    }

    return err;
}

#endif // _WIN32

#endif // _COPS_V7_EMULATOR_UNICODE_H
