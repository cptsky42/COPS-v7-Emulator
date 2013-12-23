/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 * Copyright (C) 1991 Glenn Fowler/Phong Vo/Landon Curt Noll
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_FNV_H_
#define _COPS_V7_EMULATOR_FNV_H_

#include "common.h"

class IFnv;

/**
 * The FNV_CTX structure contains the state of the FNV hash.
 * For a 32-bit version of the algorithm, State32 must be used.
 * For a 64-bit version of the algorithm, State64 must be used.
 */
struct FNV_CTX
{
public:
    union
    {
        uint32_t State32; //!< The 32-bit hash will be computed in this state.
        uint64_t State64; //!< The 64-bit hash will be computed in this state.
    };

public:
    /* constructor */
    FNV_CTX() { State32 = 0UL; State64 = 0ULL; }

    /* destructor */
    ~FNV_CTX() { }
};

/**
 * Fowler-Noll-Vo (FNV) is a non-cryptographic hash function created by
 * Glenn Fowler, Landon Curt Noll, and Phong Vo.
 *
 * The FNV hash algorithms and sample sources have been released into the
 * public domain.
 *
 * FNV hashes are designed to be fast while maintaining a low collision
 * rate. The FNV speed allows one to quickly hash lots of data while
 * maintaining a reasonable collision rate. The high dispersion of the
 * FNV hashes makes them well suited for hashing nearly identical strings
 * such as URLs, hostnames, filenames, text, IP addresses, etc.
 *
 * This implementation supports the FNV1 and FNV1a algorithms in their
 * respective 32-bit and 64-bit form.
 */
class Fnv
{
public:
    /** All the available modes to compute a FNV hash. */
    enum Mode
    {
        /** Compute a 32-bit hash */
        FNV1_32 = 1,
        /** Compute a 32-bit hash with the XOR before the multiplication */
        FNV1a_32,
        /** Compute a 64-bit hash */
        FNV1_64,
        /** Compute a 64-bit hash with the XOR before the multiplication */
        FNV1a_64
    };

    /** The 32-bit prime used by the 32-bit versions of the hash algorithm */
    static const uint32_t PRIME32 = UINT32_C(16777619);
    /** The 32-bit base used by the 32-bit versions of the hash algorithm */
    static const uint32_t OFFSET_BASIS32 = UINT32_C(2166136261);
    /** The 64-bit prime used by the 64-bit versions of the hash algorithm */
    static const uint64_t PRIME64 = UINT64_C(1099511628211);
    /** The 64-bit base used by the 64-bit versions of the hash algorithm */
    static const uint64_t OFFSET_BASIS64 = UINT64_C(14695981039346656037);

    // N.B. The offset basis is the FNV0 hash of the string:
    //      "chongo <Landon Curt Noll> /\../\"
    //      The FNV0 algorithm uses an offset basis of 0.
    // N.B. For more informations on these constants, please check the official
    //      website: http://www.isthe.com/chongo/tech/comp/fnv/index.html

public:
    /* constructor */
    Fnv();

    /* destructor */
    ~Fnv();

    /**
     * Inits the FNV state.
     * It should be called before computing.
     *
     * @param[in] aMode (optional) Specify the algorithm to use.
     *                             Defaults to FNV1_32.
     */
    err_t init(Mode aMode = FNV1_32);

    /**
     * Computes n octet(s) with the specified mode.
     *
     * @param[in]  aBuf       a pointer to the buffer to compute
     * @param[in]  aLen       the number of octets of the buffer
     */
    err_t compute(uint8_t* aBuf, size_t aLen);

    /**
     *Fianlizes the result.
     * It should be called before getting the computed hash.
     *
     * @returns The finalized FNV_CTX structure.
     */
    FNV_CTX finalize();

    /**
     * Gets the actual FNV state.
     *
     * @returns The actual FNV_CTX structure.
     */
    FNV_CTX getHash();

private:
    IFnv* mHasher;  //!< the hasher used to compute the data
    FNV_CTX mContext;   //!< the FNV state
};

#endif // _COPS_V7_EMULATOR_FNV_H_
