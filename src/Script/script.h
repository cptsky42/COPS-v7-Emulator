/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_SCRIPT_H_
#define _COPS_V7_EMULATOR_SCRIPT_H_

#include "log.h"
#include "common.h"
#include "env.h"

class Client;
struct lua_State;

/**
 * A Lua script.
 */
class Script
{
public:
    /**
     * Get the Lua VM singleton. If the object does not exist yet,
     * it will be created.
     *
     * @returns A reference to the singleton
     */
    static lua_State* getState() { return &State::getState(); }

public:
    /**
     * Register the shared functions of the scripts.
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    static err_t registerFunctions();

public:
    /* destructor */
    virtual ~Script();

    /**
     * Execute the script for the specified client and parameter.
     *
     * @param[in]  aClient   the caller
     * @param[in]  aParam    the parameter
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the Lua script failed
     * @returns Error code otherwise
     */
    virtual err_t execute(Client& aClient, int32_t aParam) const = 0;

public:
    /** Get the unique ID of the script. */
    uint32_t getUID() const { return mUID; }

protected:
    // Getters / Setters Lua methods
    static int getName(lua_State* aState);
    static int getMate(lua_State* aState);
    static int getLook(lua_State* aState);
    static int getHair(lua_State* aState);
    static int getMoney(lua_State* aState);
    static int getCPs(lua_State* aState);
    static int getExp(lua_State* aState);
    static int getForce(lua_State* aState);
    static int getHealth(lua_State* aState);
    static int getDexterity(lua_State* aState);
    static int getSoul(lua_State* aState);
    static int getAddPoints(lua_State* aState);
    static int getCurHP(lua_State* aState);
    static int getMaxHP(lua_State* aState);
    static int getCurMP(lua_State* aState);
    static int getMaxMP(lua_State* aState);
    static int getPkPoints(lua_State* aState);
    static int getLevel(lua_State* aState);
    static int getProfession(lua_State* aState);

    static int gainMoney(lua_State* aState);
    static int gainCPs(lua_State* aState);
    static int spendMoney(lua_State* aState);
    static int spendCPs(lua_State* aState);

    static int move(lua_State* aState);

    // MsgDialog Lua methods
    static int text(lua_State* aState);
    static int link(lua_State* aState);
    static int pic(lua_State* aState);
    static int create(lua_State* aState);

protected:
    /* constructor */
    Script(uint32_t aUID, const char* aPath);

protected:
    const uint32_t mUID; //!< The unique ID of the script owner.



private:
    /**
     * The global Lua VM wrapper.
     * It is a singleton and will be created when getting the instance.
     */
    class State : public Environment::Global
    {
        // !!! class is a singleton !!!
        PROHIBIT_COPY(State);

    public:
        /**
         * Get the Lua VM singleton. If the object does not exist yet,
         * it will be created.
         *
         * @returns A reference to the singleton
         */
        static lua_State& getState();

    public:
        /* destructor */
        virtual ~State();

    private:
        /* destructor */
        State();

    private:
        static State* sState; //!< static instance of the singleton

    private:
        lua_State* mState; //!< Lua VM state (global due to wrapper)
    };
};

#endif // _COPS_V7_EMULATOR_SCRIPT_H_
