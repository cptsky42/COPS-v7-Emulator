/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "env.h"
#include "common.h"

#include <cassert>
#include <cstdlib>

#include <algorithm>
#include <atomic>
#include <thread>

/* static */
Environment* Environment::sInstance = nullptr;

/* static */
Environment&
Environment :: getInstance()
{
    static volatile std::atomic<int> protect(0);

    if (sInstance == nullptr)
    {
        if (1 == ++protect)
        {
            // create the instance
            sInstance = new Environment();
            assert(sInstance != nullptr);
        }
        else
        {
            while (sInstance == nullptr)
                std::this_thread::yield();
        }
    }
    return *sInstance;
}

/* static */
void
Environment :: destroy()
{
    SAFE_DELETE(sInstance);
}

Environment :: Environment()
{
    ASSERT(sInstance == nullptr);
    mIsDestroying = false;

    atexit(Environment::destroy);
}

Environment :: ~Environment()
{
    assert(this == sInstance);
    mIsDestroying = true;

    for (auto obj : mObjects)
        SAFE_DELETE(obj);
    mObjects.clear();

    sInstance = nullptr;
}

void
Environment :: registerObj(Global* aObj)
{
    assert(aObj != nullptr);
    mObjects.push_back(aObj);
}

void
Environment :: unregisterObj(Global* aObj)
{
    assert(aObj != nullptr);

    if (!mIsDestroying)
    {
        auto it = std::find(mObjects.begin(), mObjects.end(), aObj);

        if (mObjects.end() != it)
            mObjects.erase(it);
    }
}

