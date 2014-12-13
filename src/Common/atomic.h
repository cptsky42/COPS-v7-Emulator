/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_ATOMIC_H_
#define _COPS_V7_EMULATOR_ATOMIC_H_

#include "common.h"

#if defined(TARGET_SYSTEM_MACOS_X)
#   include <libkern/OSAtomic.h>
#elif defined(TARGET_SYSTEM_WINDOWS)
#   include <windows.h>
#endif

#if defined(TARGET_SYSTEM_MACOS_X)
#   if defined(TARGET_INSTR_X86_64) || defined(TARGET_INSTR_PPC64)
        typedef int64_t atomic_t;
#   else
        typedef int32_t atomic_t;
#   endif // x86_64 || ppc64
#elif defined(TARGET_SYSTEM_WINDOWS)
    typedef LONG atomic_t;
#elif defined(__GNUC__)
    typedef long int atomic_t;
#else
#   error "Need more porting work for atomic_t."
#endif

/**
 * Increments (increases by one) the value of the specified
 * integer variable as an atomic operation.
 *
 * @param[in]  ptr  a pointer to the variable to be incremented.
 *
 * @returns the resulting incremented value
 */
static __forceinline atomic_t atomic_inc(volatile atomic_t* ptr)
{
    #if defined(TARGET_SYSTEM_MACOS_X)
    #   if defined(TARGET_INSTR_X86_64) || defined(TARGET_INSTR_PPC64)
            return OSAtomicIncrement64Barrier(ptr);
    #   else
            return OSAtomicIncrement32Barrier(ptr);
    #   endif // x86_64 || ppc64
    #elif defined(TARGET_SYSTEM_WINDOWS)
            return InterlockedIncrement(ptr);
    #elif defined(__GNUC__)
            return (__sync_fetch_and_add(ptr, 1) + 1);
    #else
    #   error "Need more porting work for atomic_inc."
    #endif
}

#endif // _COPS_V7_EMULATOR_ATOMIC_H_
