/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_COMMON_H_
#define _COPS_V7_EMULATOR_COMMON_H_

#include "arch.h"
#include "types.h"
#include "endianness.h"
#include "err.h"
#include "basefunc.h"
#include "strres.h"

#ifdef _WIN32
#define NOMINMAX // want std::min() & std::max() defined...
#include <windows.h>

#ifdef _MSC_VER // Visual Studio will complain for linking...
#pragma comment(lib, "WinMM") // for timeGetTime()
#endif

#else
#include <unistd.h> // sysctl, sysconf
#include <sys/time.h> // for timeGetTime()
#endif

#ifdef __APPLE__
#include <libkern/OSAtomic.h>
#endif // atomics

// Clang defines __has_feature
#ifndef __has_feature
#define __has_feature(x) 0
#endif // __has_feature

/*
 *****************************************************
 * Safe delete macros
 ****************************************************
 */

// Delete a pointer and set the pointer to NULL.
#define SAFE_DELETE(ptr)                          \
    delete ptr,                                   \
    ptr = nullptr;

// Delete an array and set the pointer to NULL.
#define SAFE_DELETE_ARRAY(ptr)                    \
    delete[] ptr,                                 \
    ptr = nullptr;

/*
 *****************************************************
 * Protection macros
 ****************************************************
 */

// Prohibit the copy of the object
#define PROHIBIT_COPY(__class)                   \
    private:                                        \
        __class(const __class&);                    \
        __class& operator=(const __class&)

/*
 *****************************************************
 * Common macros
 ****************************************************
 */

// Quote strings in macro */
#define STRINGIFY_(str) #str
#define STRINGIFY(str) STRINGIFY_(str)

// GCC 4.3+, MSVC 2010+ or Clang feature-detection
#if !(defined(__GNUC__) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 430)) && \
    !(defined(__clang__) && __has_feature(cxx_rvalue_references)) && \
    !(defined(_MSC_VER) && (_MSC_VER >= 1600))
#define HAS_CXX_RVALUE_REF
#endif

/*
 *****************************************************
 * « TaskManager » functions
 ****************************************************
 */

// Get the number of available CPU / cores...
inline int getNumCPU()
{
    int numCPU = 1;

    #if defined(TARGET_SYSTEM_WINDOWS)
    // Windows... Can use Windows API...
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    numCPU = sysinfo.dwNumberOfProcessors;
    #elif defined(TARGET_SYSTEM_MACOS_X) || defined(TARGET_SYSTEM_GNU_LINUX) || \
          defined(TARGET_SYSTEM_AIX) || defined(TARGET_SYSTEM_SOLARIS)
    // Linux, Solaris, AIX & Mac OS X (Tiger onwards)
    numCPU = sysconf(_SC_NPROCESSORS_ONLN);
    #else
    // Probably Unix-like (FreeBSD, NetBSD, etc)
    int mib[4];
    size_t len = sizeof(numCPU);

    /* set the mib for hw.ncpu */
    mib[0] = CTL_HW;
    mib[1] = HW_AVAILCPU;  // alternatively, try HW_NCPU;

    /* get the number of CPUs from the system */
    sysctl(mib, 2, &numCPU, &len, NULL, 0);

    if (numCPU < 1) // HW_AVAILCPU might be incorrect
    {
         mib[1] = HW_NCPU;
         sysctl(mib, 2, &numCPU, &len, NULL, 0);
    }
    #endif

    if (numCPU < 1)
    {
        numCPU = 1;
    }

    return numCPU;
}

/*
 *****************************************************
 * Cross-compiling definitions
 ****************************************************
 */

// If using Visual Studio, __FUNCTION__ is not defined, but __func__ is
#ifndef _MSC_VER
#define __FUNCTION__ __func__
#endif // _MSC_VER

// If using Visual Studio, and C++11 is not implemented
// POSIX-compliant platforms define those functions, so C++11 is not required
#if defined(_MSC_VER) && __cplusplus < 201103L
#define snprintf _snprintf
#endif

// If __TIMESTAMP__ is not defined, create it by merging __DATE__ and __TIME__
#ifndef __TIMESTAMP__
#define __TIMESTAMP__ __DATE__" "__TIME__
#endif

// If not on Windows, we must implement a kind-of timeGetTime()
#ifndef _WIN32
inline unsigned int timeGetTime()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    return (now.tv_sec * 1000) + (now.tv_usec / 1000);
}
#endif

#if defined(__APPLE__)
#   if defined(TARGET_INSTR_X86_64) || defined(TARGET_INSTR_PPC64)
#       define atomic_inc(ptr) OSAtomicIncrement64Barrier(((volatile int64_t*)ptr))
#   else
#       define atomic_inc(ptr) OSAtomicIncrement32Barrier(((volatile int32_t*)ptr))
#   endif
#elif defined (_WIN32)
#   define atomic_inc(ptr) InterlockedIncrement((ptr))
#elif defined(__GNUC__)
#   define atomic_inc(ptr) (__sync_fetch_and_add((ptr), 1) + 1)
#else
#   error "Need some more porting work for atomic_inc."
#endif

#ifndef _WIN32
#define mssleep(ms) usleep(ms * 1000)
#else
#define mssleep(ms) Sleep(ms)
#endif // _WIN32

#endif // _COPS_V7_EMULATOR_COMMON_H_
