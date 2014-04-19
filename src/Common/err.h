/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_ERROR_H_
#define _COPS_V7_EMULATOR_ERROR_H_

#include "types.h"
#include <assert.h>
#include <errno.h>

/*
 *****************************************************
 * Errors definitions
 *****************************************************
 */

typedef int32_t err_t;

/** The operation completed successfully */
const err_t ERROR_SUCCESS = 0;

/** Invalid function. */
const err_t ERROR_INVALID_FUNCTION = 1;

/** Access is denied. */
const err_t ERROR_ACCESS_DENIED = 2;

/** Handle is invalid */
const err_t ERROR_INVALID_HANDLE = 3;

/** Pointer is invalid */
const err_t ERROR_INVALID_POINTER = 4;

/** Reference is invalid */
const err_t ERROR_INVALID_REFERENCE = 5;

/** Data error */
const err_t ERROR_BAD_CHECKSUM = 6;

/** The length is incorrect. */
const err_t ERROR_BAD_LENGTH = 7;

/** The parameter is incorrect */
const err_t ERROR_INVALID_PARAMETER = 8;

/** The data is too long */
const err_t ERROR_BUFFER_OVERFLOW = 9;

/** The element does not exist. */
const err_t ERROR_NOT_FOUND = 10;

/** The state is incorrect. */
const err_t ERROR_INVALID_STATE = 11;


/** The specified password is incorrect */
const err_t ERROR_INVALID_PASSWORD = 100;


/** Not enough memory to process the operation */
const err_t ERROR_NOT_ENOUGH_MEMORY = 500;

/** An attempt was made to load a program with an incorrect format */
const err_t ERROR_BAD_FORMAT = 501;


/** Cannot find the specified file */
const err_t ERROR_FILE_NOT_FOUND = 1000;

/** Caonnot find the specified path */
const err_t ERROR_PATH_NOT_FOUND = 1001;

/** Cannot open the file. Too many files open. */
const err_t ERROR_TOO_MANY_FILES_OPEN = 1002;

/** Cannot locate a specific area on the disk */
const err_t ERROR_SEEK = 1003;

/** Cannot write to the specified disk. */
const err_t ERROR_WRITE_FAULT = 1004;

/** Cannot read from the specified disk. */
const err_t ERROR_READ_FAULT = 1005;

/** Cannot access the file because another process has locked the file. */
const err_t ERROR_LOCK_VIOLATION = 1006;

/** Cannot open the file specified. */
const err_t ERROR_OPEN_FAILED = 1007;

/** Cannot create the file or directory */
const err_t ERROR_CANNOT_CREATE = 1008;

/** The file exists. */
const err_t ERROR_FILE_EXISTS = 1009;

/** The disk is full */
const err_t ERROR_DISK_FULL = 1010;


/** Cannot execute the specified command. */
const err_t ERROR_EXEC_FAILED = 2000;


/** Unknown error */
const err_t ERROR_UNKNOWN = 0x7FFFFFFF;

/*
 *****************************************************
 * Error checking macros
 *****************************************************
 */

// Return TRUE if the error is success
#define IS_SUCCESS(err) \
    (ERROR_SUCCESS == err)

// If the error is success, execute the function and update the error
#define DOIF(err, fn) \
    if (IS_SUCCESS(err)) \
    err = fn

/*
 *****************************************************
 * Assertion macros
 *****************************************************
 */

#ifndef NDEBUG

// Assert an expression and might abort
#define ASSERT(exp)                          \
    assert(exp)

// Assert an expression and might abort
#define ASSERT_ERR(exp, err)                 \
    assert(exp)

#else

// Assert an expression and return if false
#define ASSERT(exp)                          \
    if (!(exp)) { }

// Assert an expression and return the error if false
#define ASSERT_ERR(exp, err)                 \
    if (!(exp))                              \
        return err

#endif // DEBUG

#endif // _COPS_V7_EMULATOR_ERROR_H_
