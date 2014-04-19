/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "npctask.h"
#include "client.h"
#include "lua.hpp"

NpcTask :: NpcTask(uint32_t aUID, const char* aPath)
    : Script(aUID, aPath)
{
    char fct[128];
    snprintf(fct, sizeof(fct), "processTask%u", aUID);

    mFct = fct;
}

NpcTask :: ~NpcTask()
{

}

err_t
NpcTask :: execute(Client& aClient, int32_t aParam) const
{
    err_t err = ERROR_SUCCESS;

    lua_State* state = Script::getState();

    LOG(INFO, "Executing npc task %d with client=%p, param=%d.",
        mUID, &aClient, aParam);

    lua_getglobal(state, mFct.c_str());
    lua_pushinteger(state, (ptrdiff_t)&aClient);
    lua_pushinteger(state, aParam);
    LOG(DBG, "Calling function %s", mFct.c_str());

    int luaerr = lua_pcall(state, 2, 0, 0); // state, nargs, nret, err_fct
    if (LUA_OK != luaerr)
    {
        LOG(ERROR, "Lua error %d calling %s:\n %s",
              luaerr, mFct.c_str(), lua_tostring(state, -1));
        err = ERROR_EXEC_FAILED;
    }

    return err;
}
