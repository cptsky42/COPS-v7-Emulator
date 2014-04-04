/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_ENDIAN_H_
#define _COPS_V7_EMULATOR_ENDIAN_H_

#include "types.h"

// Clang defines __has_builtin
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif // __has_builtin

/*
 *****************************************************
 * Determine endian at compilation
 ****************************************************
 */

// If BYTE_ORDER is defined, it should be equal to LITTLE_ENDIAN or BIG_ENDIAN.
// Else, it's an unsupported endianness.
// The value of LITTLE_ENDIAN and BIG_ENDIAN may be different to the one defined in this header.
#if defined(BYTE_ORDER) && LITTLE_ENDIAN != BIG_ENDIAN

#if (BYTE_ORDER != LITTLE_ENDIAN && BYTE_ORDER != BIG_ENDIAN)
#error Unsupported endianness!
#endif

#elif defined(__BYTE_ORDER) && __LITTLE_ENDIAN != __BIG_ENDIAN

// already defined with some header/compiler
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#define BIG_ENDIAN __BIG_ENDIAN

// If __BYTE_ORDER__ is defined, it is possible to use it to define BYTE_ORDER with our custom values.
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define BYTE_ORDER LITTLE_ENDIAN
#elif  __BYTE_ORDER == __BIG_ENDIAN
#define BYTE_ORDER BIG_ENDIAN
#else
#error Unsupported endianness!
#endif

#elif defined(__BYTE_ORDER__) && __ORDER__LITTLE_ENDIAN__ != __ORDER__BIG_ENDIAN__

// already defined with some header/compiler
#define LITTLE_ENDIAN __ORDER__LITTLE_ENDIAN__
#define BIG_ENDIAN __ORDER__BIG_ENDIAN__

// If __BYTE_ORDER__ is defined, it is possible to use it to define BYTE_ORDER with our custom values.
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BYTE_ORDER LITTLE_ENDIAN
#elif  __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define BYTE_ORDER BIG_ENDIAN
#else
#error Unsupported endianness!
#endif

#else

#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN 4321

// If no byte order is defined, it is possible to use the target architecture macro to define
// BYTE_ORDER with our custom values.
// For the Bi-Endian architecture (e.g. ARM, IA64, etc), the default architecture is used.
// Although it SHOULD works in most cases, using runtime checks is more safer.
#if defined(__i386__) || defined(__i386) || defined(__X86__) || defined(_X86_) || defined(_M_IX86) || \
    defined(__x86_64__) || defined(__amd64__) || defined(__x86_64) || defined(__amd64) || \
    defined(_M_X64) || defined(_M_AMD64) ||                             \
    defined(__arm__) || defined(_ARM) ||                                \
    defined(__mips__) || defined(__MIPS__) || defined(__mips) ||        \
    defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
#define BYTE_ORDER LITTLE_ENDIAN
#elif defined(__ppc__) || defined(__ppc64__) || defined(__powerpc__) || defined(_M_MPPC) || \
    defined(_M_PPC) ||                                                  \
    defined(__sparc__) || defined(__sparc) ||                           \
    defined(__ia64__) || defined(__IA64__) || defined(__ia64) || defined(__IA64)  || \
    defined(__itanium__) ||defined(_M_IA64) ||                          \
    defined(__hppa__) || defined (__hppa) || defined(__HPPA__) ||       \
    defined(__m68k__) || defined(__MC68K__) || defined(M68000)
#define BYTE_ORDER BIG_ENDIAN
#else
#error Unknow endianness!
#endif // Architecture

#endif // BYTE_ORDER

/*
 *****************************************************
 * Byte swapping methods
 ****************************************************
 */

/**
 * Swap all the bytes of the 16-bit integer to get the value
 * of the opposite endianness.
 *
 * @param[in]  x      the 16-bit integer
 *
 * @returns the integer with all bytes swapped for the opposite endian
 */
inline int16_t bswap16(int16_t x)
{
    // GCC 4.8+ or Clang
    #if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 480)) || \
        (defined(__clang__) && __has_builtin(__builtin_bswap16))
    return __builtin_bswap16(x);
    #else
    return ((((uint16_t)(x) & 0xFF00) >> 8) | \
            (((uint16_t)(x) & 0x00FF) << 8));
    #endif
}

/**
 * Swap all the bytes of the 16-bit unsigned integer to get the value
 * of the opposite endianness.
 *
 * @param[in]  x      the 16-bit unsigned integer
 *
 * @returns the integer with all bytes swapped for the opposite endian
 */
inline uint16_t bswap16(uint16_t x)
{
    return (uint16_t)bswap16((int16_t)x);
}

/**
 * Swap all the bytes of the 32-bit integer to get the value
 * of the opposite endianness.
 *
 * @param[in]  x      the 32-bit integer
 *
 * @returns the integer with all bytes swapped for the opposite endian
 */
inline int32_t bswap32(int32_t x)
{
    // GCC 4.3+ or Clang
    #if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 430)) || \
        (defined(__clang__) && __has_builtin(__builtin_bswap32))
    return __builtin_bswap32(x);
    #else
    return ((((uint32_t)(x) & 0xFF000000) >> 24) |                 \
            (((uint32_t)(x) & 0x00FF0000) >> 8) |                  \
            (((uint32_t)(x) & 0x0000FF00) << 8) |                  \
            (((uint32_t)(x) & 0x000000FF) << 24));
    #endif
}

/**
 * Swap all the bytes of the 32-bit unsigned integer to get the value
 * of the opposite endianness.
 *
 * @param[in]  x      the 32-bit unsigned integer
 *
 * @returns the integer with all bytes swapped for the opposite endian
 */
inline uint32_t bswap32(uint32_t x)
{
    return (uint32_t)bswap32((int32_t)x);
}

/**
 * Swap all the bytes of the 64-bit integer to get the value
 * of the opposite endianness.
 *
 * @param[in]  x      the 64-bit integer
 *
 * @returns the integer with all bytes swapped for the opposite endian
 */
inline int64_t bswap64(int64_t x)
{
    // GCC 4.3+ or Clang
    #if (defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 430)) || \
        (defined(__clang__) && __has_builtin(__builtin_bswap64))
    return __builtin_bswap64(x);
    #else
    return (((uint64_t)(x) << 56) |                         \
            (((uint64_t)(x) << 40) & 0xFF000000000000ULL) | \
            (((uint64_t)(x) << 24) & 0xFF0000000000ULL) |   \
            (((uint64_t)(x) << 8)  & 0xFF00000000ULL) |     \
            (((uint64_t)(x) >> 8)  & 0xFF000000ULL) |       \
            (((uint64_t)(x) >> 24) & 0xFF0000ULL) |         \
            (((uint64_t)(x) >> 40) & 0xFF00ULL) |           \
            ((uint64_t)(x)  >> 56));
    #endif
}

/**
 * Swap all the bytes of the 64-bit unsigned integer to get the value
 * of the opposite endianness.
 *
 * @param[in]  x      the 64-bit unsigned integer
 *
 * @returns the integer with all bytes swapped for the opposite endian
 */
inline uint64_t bswap64(uint64_t x)
{
    return (uint64_t)bswap64((int64_t)x);
}

#endif // _COPS_V7_EMULATOR_ENDIAN_H
