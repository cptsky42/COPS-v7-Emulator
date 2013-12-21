/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_NPC_TASK_H_
#define _COPS_V7_EMULATOR_NPC_TASK_H_

#include "common.h"
#include "script.h"
#include <string>

class Client;
struct lua_State;

/**
 * Process a Lua script and execute the processTask when executed.
 * It is used by NPC's tasks.
 */
class NpcTask : public Script
{
public:
    /**
     * Register the specific functions of the NPC's tasks.
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    static err_t registerFunctions();

public:
    /**
     * Create a new task to be used by a NPC.
     *
     * @param[in]  aUID    the unique ID of the script
     * @param[in]  aPath   the path of the Lua script
     */
    NpcTask(uint32_t aUID, const char* aPath);

    /* destructor */
    virtual ~NpcTask();

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
    virtual err_t execute(Client& aClient, int32_t aParam) const;

private:
    // MsgDialog Lua methods
    static int text(lua_State* aState);
    static int link(lua_State* aState);
    static int pic(lua_State* aState);
    static int create(lua_State* aState);

private:
    std::string mFct; //!< the function's name to call
};

#endif // _COPS_V7_EMULATOR_NPC_TASK_H_
