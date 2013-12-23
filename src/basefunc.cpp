/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "common.h"
#include "basefunc.h"
#include <math.h>

int random(int aMax, bool aReset)
{
    static unsigned long seed = timeGetTime();

    if (aReset)
    {
        seed = timeGetTime();
    }

    unsigned long x = UINT_MAX;
    double i;
    unsigned long final;

    seed *= 134775813;
    seed += 1;
    seed = seed % x;

    i = ((double)seed) / (double)x;
    final = (unsigned long)(aMax * i);

    return (int)final;
}

double randomRate(double aRange)
{
    const double PI = 3.1415926;

    int rand = random(999, false) + 1;
    double a = sin(rand * PI / 1000.0);
    double b;

    if (rand >= 90)
    {
        b = (1.0 + aRange) - sqrt(sqrt(a)) * aRange;
    }
    else
    {
        b = (1.0 - aRange) + sqrt(sqrt(a)) * aRange;
    }

    return b;
}

bool isValidString(const char* aStr)
{
    if (aStr == nullptr)
        return false;

    for (size_t i = 0, len = strlen(aStr); i < len; ++i)
    {
        unsigned char c = aStr[i];
        if (c >= 0x81 && c <= 0xFE)
        {
            if (i + 1 >= len)
                return false;

            unsigned char c2 = aStr[i + 1];
            if (c2 < 0x40 && c2 > 0x7E && c2 < 0x80 && c2 > 0xFE)
                return false;
            else
                ++i;
        }
        else
        {
            if (c == 0x80)
                return false;

            if (c < ' ')
                return false;
        }
    }

    return true;
}

bool isValidMsgString(const char* aStr)
{
    if (aStr == nullptr)
        return false;

    for (size_t i = 0, len = strlen(aStr); i < len; ++i)
    {
        unsigned char c = aStr[i];

        if (c < ' ')
            return false;

        switch (c)
        {
            case '\\':
            case '\'':
            case '"':
                return false;
        }
    }

    return isValidString(aStr);
}

bool isValidNameString(const char* aStr)
{
    if (aStr == nullptr)
        return false;

    for (size_t i = 0, len = strlen(aStr); i < len; ++i)
    {
        unsigned char c = aStr[i];

        if (c < ' ')
            return false;

        switch (c)
        {
            case ' ':
            case ';':
            case ',':
            case '/':
            case '\\':
            case '=':
            case '%':
            case '@':
            case '\'':
            case '"':
            case '[':
            case ']':
                return false;
        }
    }

    if (strcmp(aStr, "None")  == 0)
        return false;

    if (strstr(aStr, "GM") != nullptr)
        return false;

    if (strstr(aStr, "gm") != nullptr)
        return false;

    if (strstr(aStr, "PM") != nullptr)
        return false;

    if (strstr(aStr, "pm") != nullptr)
        return false;

    if (strstr(aStr, "SYSTEM") != nullptr)
        return false;

    if (strstr(aStr, "system") != nullptr)
        return false;

    if (strstr(aStr, "NPC") != nullptr)
        return false;

    if (strstr(aStr, "npc") != nullptr)
        return false;

    return isValidString(aStr);
}
