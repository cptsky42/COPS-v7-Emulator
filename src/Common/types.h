/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_TYPES_H_
#define _COPS_V7_EMULATOR_TYPES_H_

#include <stddef.h> // Define NULL, size_t
#include <limits.h> // Define UCHAR_MAX, etc

// Clang defines __has_feature
#ifndef __has_feature
#define __has_feature(x) 0
#endif // __has_feature

/*
 *****************************************************
 * Fixed-width integer definitions
 ****************************************************
 */
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS
#include <stdint.h> // Will define intN_t

/*
 *****************************************************
 * nullptr definition
 ****************************************************
 */

// GCC 4.6+, MSVC 2010+ or Clang feature-detection
#if !(defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 460)) && \
    !(defined(__clang__) && __has_feature(cxx_nullptr)) && \
    !(defined(_MSC_VER) && (_MSC_VER >= 1600))
#define nullptr NULL
#endif

/*
 *****************************************************
 * constexpr definition
 ****************************************************
 */

// GCC 4.6+ or Clang feature-detection
#if !(defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 460)) && \
    !(defined(__clang__) && __has_feature(cxx_constexpr))
#define constexpr const
#endif

#endif // _COPS_V7_EMULATOR_TYPES_H
