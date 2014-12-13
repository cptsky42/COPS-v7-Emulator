/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_DEF_H_
#define _COPS_V7_EMULATOR_DEF_H_

// Clang defines __has_feature
#ifndef __has_feature
#define __has_feature(x) 0
#endif // __has_feature

// Clang defines __has_builtin
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif // __has_builtin

#ifdef _WIN32
#define NOMINMAX // want std::min() & std::max() defined...
#define WIN32_LEAN_AND_MEAN // want minimal WinAPI...
#endif // _WIN32

// For common compilers, only MSVC 2008- doesn't have <stdint.h>
#if !defined(_MSC_VER) || (defined(_MSC_VER) && _MSC_VER >= 1600)
#define HAVE_STDINT_H
#endif

//#cmakedefine HAVE_UNISTD_H
//#cmakedefine HAVE_ENDIAN_H
//#cmakedefine HAVE_PTHREAD_H
//#cmakedefine HAVE_SCHED_H


//#cmakedefine HAVE_USLEEP_FN
//#cmakedefine HAVE_SCHED_YIELD_FN
//#cmakedefine HAVE_GETTIMEOFDAY_FN

// GCC 4.3+, MSVC 2010+ or Clang feature-detection
#if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 430)) || \
    (defined(__clang__) && __has_feature(cxx_rvalue_references)) || \
    (defined(_MSC_VER) && (_MSC_VER >= 1600))
#define HAVE_CXX_RVALUE_REF 1
#endif

// GCC 4.6+, MSVC 2010+ or Clang feature-detection
#if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 460)) || \
    (defined(__clang__) && __has_feature(cxx_nullptr)) || \
    (defined(_MSC_VER) && (_MSC_VER >= 1600))
#define HAVE_CXX_NULLPTR 1
#endif

// GCC 4.6+ or Clang feature-detection
// MSVC 2012+ does not support constexpr, but blocks its macroization.
#if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 460)) || \
    (defined(__clang__) && __has_feature(cxx_constexpr)) || \
    (defined(_MSC_VER) && (_MSC_VER >= 1700))
#define HAVE_CXX_CONSTEXPR 1
#endif

// GCC 4.3+, MSVC 2010+ or Clang feature-detection
#if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 430)) || \
    (defined(__clang__) && __has_feature(cxx_static_assert)) || \
    (defined(_MSC_VER) && (_MSC_VER >= 1600))
#define HAVE_CXX_STATIC_ASSERT 1
#endif


// GCC 4.8+ or Clang
#if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 480)) || \
    (defined(__clang__) && __has_builtin(__builtin_bswap16))
#define HAVE_BUILTIN_BSWAP16
#endif

// GCC 4.3+ or Clang
#if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 430)) || \
    (defined(__clang__) && __has_builtin(__builtin_bswap32))
#define HAVE_BUILTIN_BSWAP32
#endif

// GCC 4.3+ or Clang
#if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 430)) || \
    (defined(__clang__) && __has_builtin(__builtin_bswap64))
#define HAVE_BUILTIN_BSWAP64
#endif

#endif // _COPS_V7_EMULATOR_DEF_H_
