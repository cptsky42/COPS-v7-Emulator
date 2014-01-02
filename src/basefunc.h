/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_BASEFUNC_H_
#define _COPS_V7_EMULATOR_BASEFUNC_H_

#include "common.h"

/**
 * Convert a character to an integer.
 *
 * @param[in]   aInput    the character to convert
 *
 * @returns the integer value represented by the character
 */
uint8_t char2int(char aInput);

/**
 * Convert an hexadecimal string to binary.
 *
 * @param[in]   aHexStr
 * @param[in]   aOutBytes
 * @param[in]   aOutLen
 *
 * @retval TRUE on success
 * @returns FALSE otherwise
 */
bool hex2bin(const char* aHexStr, uint8_t* aOutBytes, size_t& aOutLen);

/**
 * Random algorithm based on the one used by TQ in their servers.
 *
 * @param[in]   aMin       the minimal value
 * @param[in]   aMax       the maximal value
 * @param[in]   aReset     if the algorithm must be reseeded
 *
 * @returns a value between aMin and aMax (with both extreme included)
 */
int random(int aMin, int aMax, bool aReset = false);

/**
 * Random rate algorithme based on the one used by TQ in their servers.
 * It is used at least for the durability of dropped items (range is 0.3).
 *
 * @param[in]    aRange     the range of the algorithm
 *
 * @returns a value in the specified range
 */
double randomRate(double aRange);

/**
 * Determine whether a string is valid or not.
 *
 * @param[in]   aStr    the string to validate
 *
 * @retval TRUE if the string is valid
 * @returns FALSE otherwise
 */
bool isValidString(const char* aStr);

/**
 * Determine whether a string is valid for a message or not.
 *
 * @param[in]   aStr    the string to validate
 *
 * @retval TRUE if the string is valid
 * @returns FALSE otherwise
 */
bool isValidMsgString(const char* aStr);

/**
 * Determine whether a string is valid for a name or not.
 *
 * @param[in]   aStr    the string to validate
 *
 * @retval TRUE if the string is valid
 * @returns FALSE otherwise
 */
bool isValidNameString(const char* aStr);

#endif // _COPS_V7_EMULATOR_BASEFUNC_H_
