/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 * Copyright (C) 1991 Glenn Fowler/Phong Vo/Landon Curt Noll
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "fnv.h"

/**
 * Internal definition for Fnv hashes
 */
class IFnv
{
public:
    static err_t makeFnv(IFnv** aOutFnv, FNV_CTX& aContext,
                           Fnv::Mode aMode = Fnv::FNV1_32);

public:
    virtual ~IFnv() { }

public:
    virtual void init() = 0;
    virtual void compute(uint8_t* aBuf, size_t aLen) = 0;

protected:
    IFnv(FNV_CTX& aContext) : mContext(aContext) { }

protected:
    FNV_CTX& mContext;
};

class Fnv1_32 : public IFnv
{
    friend class IFnv; // for the factory

public:
    virtual ~Fnv1_32() { }

public:
    virtual void init()
    {
        mContext.State32 = Fnv::OFFSET_BASIS32;
    }

    virtual void compute(uint8_t* aBuf, size_t aLen)
    {
        for (size_t i = 0; i < aLen; ++i)
        {
            mContext.State32 *= Fnv::PRIME32;
            mContext.State32 ^= aBuf[i];
        }
    }

private:
    Fnv1_32(FNV_CTX& aContext) : IFnv(aContext) { }
};

class Fnv1a_32 : public IFnv
{
    friend class IFnv; // for the factory

public:
    virtual ~Fnv1a_32() { }

public:
    virtual void init()
    {
        mContext.State32 = Fnv::OFFSET_BASIS32;
    }

    virtual void compute(uint8_t* aBuf, size_t aLen)
    {
        for (size_t i = 0; i < aLen; ++i)
        {
            mContext.State32 ^= aBuf[i];
            mContext.State32 *= Fnv::PRIME32;
        }
    }

private:
    Fnv1a_32(FNV_CTX& aContext) : IFnv(aContext) { }
};

class Fnv1_64 : public IFnv
{
    friend class IFnv; // for the factory

public:
    virtual ~Fnv1_64() { }

public:
    virtual void init()
    {
        mContext.State64 = Fnv::OFFSET_BASIS64;
    }

    virtual void compute(uint8_t* aBuf, size_t aLen)
    {
        for (size_t i = 0; i < aLen; ++i)
        {
            mContext.State64 *= Fnv::PRIME64;
            mContext.State64 ^= aBuf[i];
        }
    }

private:
    Fnv1_64(FNV_CTX& aContext) : IFnv(aContext) { }
};

class Fnv1a_64 : public IFnv
{
    friend class IFnv; // for the factory

public:
    virtual ~Fnv1a_64() { }

public:
    virtual void init()
    {
        mContext.State64 = Fnv::OFFSET_BASIS64;
    }

    virtual void compute(uint8_t* aBuf, size_t aLen)
    {
        for (size_t i = 0; i < aLen; ++i)
        {
            mContext.State64 ^= aBuf[i];
            mContext.State64 *= Fnv::PRIME64;
        }
    }

private:
    Fnv1a_64(FNV_CTX& aContext) : IFnv(aContext) { }
};

/////////////////////////////////////////////////////////////////

/* static */
err_t
IFnv :: makeFnv(IFnv** aOutFnv, FNV_CTX& aContext, Fnv::Mode aMode)
{
    ASSERT_ERR(aOutFnv != nullptr && *aOutFnv == nullptr, ERROR_INVALID_PARAMETER);

    err_t err = ERROR_SUCCESS;

    IFnv* hasher = nullptr;
    switch (aMode)
    {
    case Fnv::FNV1_32:
        {
            hasher = new Fnv1_32(aContext);
            break;
        }
    case Fnv::FNV1a_32:
        {
            hasher = new Fnv1a_32(aContext);
            break;
        }
    case Fnv::FNV1_64:
        {
            hasher = new Fnv1_64(aContext);
            break;
        }
    case Fnv::FNV1a_64:
        {
            hasher = new Fnv1a_64(aContext);
            break;
        }
    default:
        {
            err = ERROR_NOT_FOUND;
            break;
        }
    }

    if (IS_SUCCESS(err))
    {
        *aOutFnv = hasher;
        hasher = nullptr;
    }
    SAFE_DELETE(hasher);

    return err;
}

/////////////////////////////////////////////////////////////////

Fnv :: Fnv()
    : mHasher(nullptr)
{
    mContext.State32 = 0L;
    mContext.State64 = 0LL;
}

Fnv :: ~Fnv()
{
    SAFE_DELETE(mHasher);
}

err_t
Fnv :: init(Mode aMode)
{
    err_t err = ERROR_SUCCESS;

    SAFE_DELETE(mHasher); // must recreate new hasher...

    err = IFnv::makeFnv(&mHasher, mContext, aMode);
    if (IS_SUCCESS(err))
    {
        mHasher->init();
    }

    return err;
}

err_t
Fnv :: compute(uint8_t* aBuf, size_t aLen)
{
    ASSERT_ERR(mHasher != nullptr, ERROR_INVALID_STATE);

    err_t err = ERROR_SUCCESS;

    mHasher->compute(aBuf, aLen);

    return err;
}

FNV_CTX
Fnv :: finalize()
{
    return mContext;
}

FNV_CTX
Fnv :: getHash()
{
    return mContext;
}

