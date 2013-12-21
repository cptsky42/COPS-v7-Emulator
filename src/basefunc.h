/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_BASEFUNC_H_
#define _COPS_V7_EMULATOR_BASEFUNC_H_

int random(int aMax, bool aReset = false);
double randomRate(double aRange);

bool isValidString(const char* aStr);
bool isValidMsgString(const char* aStr);
bool isValidNameString(const char* aStr);

#endif // _COPS_V7_EMULATOR_BASEFUNC_H_
