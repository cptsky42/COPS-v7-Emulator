/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_ENVIRONMENT_H_
#define _COPS_V7_EMULATOR_ENVIRONMENT_H_

#include "common.h"
#include <vector>

/**
 * The environment is a global object which instantiate itself when needed and
 * destroy itself at exit.
 *
 * Singleton objects inherinting from the Global class will be destroyed
 * at exit.
 */
class Environment
{
public:
    /**
     * Global interface for global classes. It will register the instance
     * in the environment and destroy the object at exit.
     */
    class Global
    {
        friend class Environment; // register/unregister

        PROHIBIT_COPY(Global); // no copy...

    public:
        /* destructor */
        virtual ~Global()
        {
            Environment& env = Environment::getInstance();
            env.unregisterObj(this);
        }

    protected:
        /* constructor */
        Global()
        {
            Environment& env = Environment::getInstance();
            env.registerObj(this);
        }
    };

public:
    /**
     * Get the environment singleton. If the object does not exist yet,
     * it will be created.
     *
     * @returns A reference to the singleton
     */
    static Environment& getInstance();

public:
    /* destructor */
    ~Environment();

private:
    /**
     * Destroy the environment. This method is called at exit.
     */
    static void destroy();

private:
    /* constructor */
    Environment();

    /**
     * Register a global object in the environment.
     *
     * @param[in]   aObj   pointer to a global object
     */
    void registerObj(Global* aObj);

    /**
     * Unregister a global object from the environment.
     *
     * @param[in]   aObj   pointer to a global object
     */
    void unregisterObj(Global* aObj);

private:
    static Environment* sInstance; //!< Global instance of the environment

private:
    std::vector<Global*> mObjects; //!< List of all global objects
    bool mIsDestroying; //!< Determine if the environment is in it's destructor
};

#endif // _COPS_V7_EMULATOR_ENVIRONMENT_H_
