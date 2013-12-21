/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "script.h"
#include "client.h"
#include "player.h"
#include "lua.hpp"

/* static */
Script::State* Script::State::sState = nullptr;

/* static */
lua_State&
Script::State :: getState()
{
    // TODO? Thread-safe
    if (sState == nullptr)
    {
        sState = new State();
    }
    return *(sState->mState);
}

Script::State :: State()
{
    mState = luaL_newstate();
    ASSERT(mState != nullptr);
    luaL_openlibs(mState);
}

Script::State :: ~State()
{
    lua_close(mState);
}




/* static */
err_t
Script :: registerFunctions()
{
    err_t err = ERROR_SUCCESS;

    lua_State* state = Script::getState();

    // Getters / Setters Lua methods
    lua_register(state, "getName", Script::getName);
    lua_register(state, "getLook", Script::getLook);
    lua_register(state, "getHair", Script::getHair);
    lua_register(state, "getMoney", Script::getMoney);
    lua_register(state, "getExp", Script::getExp);
    lua_register(state, "getForce", Script::getForce);
    lua_register(state, "getHealth", Script::getHealth);
    lua_register(state, "getDexterity", Script::getDexterity);
    lua_register(state, "getSoul", Script::getSoul);
    lua_register(state, "getAddPoints", Script::getAddPoints);
    lua_register(state, "getCurHP", Script::getCurHP);
    lua_register(state, "getMaxHP", Script::getMaxHP);
    lua_register(state, "getCurMP", Script::getCurMP);
    lua_register(state, "getMaxMP", Script::getMaxMP);
    lua_register(state, "getPkPoints", Script::getPkPoints);
    lua_register(state, "getLevel", Script::getLevel);
    lua_register(state, "getProfession", Script::getProfession);

    return err;
}

Script :: Script(uint32_t aUID, const char* aPath)
    : mUID(aUID)
{
    ASSERT(aPath != nullptr && aPath[0] != '\0');

    lua_State* state = Script::getState();
    if (luaL_dofile(state, aPath) != 0)
    {
        LOG(ERROR, "Failed to parse Lua script `%s`:\n%s",
            aPath, lua_tostring(state, -1));
    }
}

Script :: ~Script()
{

}



/* static */
int
Script :: getName(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushstring(aState, player.getName());
    return 1;
}

/* static */
int
Script :: getLook(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getLook());
    return 1;
}

/* static */
int
Script :: getHair(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getHair());
    return 1;
}

/* static */
int
Script :: getMoney(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getMoney());
    return 1;
}

/* static */
int
Script :: getExp(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getExp());
    return 1;
}

/* static */
int
Script :: getForce(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getForce());
    return 1;
}

/* static */
int
Script :: getHealth(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getHealth());
    return 1;
}

/* static */
int
Script :: getDexterity(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getDexterity());
    return 1;
}

/* static */
int
Script :: getSoul(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getSoul());
    return 1;
}

/* static */
int
Script :: getAddPoints(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getAddPoints());
    return 1;
}

/* static */
int
Script :: getCurHP(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getCurHP());
    return 1;
}

/* static */
int
Script :: getMaxHP(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getMaxHP());
    return 1;
}

/* static */
int
Script :: getCurMP(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getCurMP());
    return 1;
}

/* static */
int
Script :: getMaxMP(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getMaxMP());
    return 1;
}

/* static */
int
Script :: getPkPoints(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getPkPoints());
    return 1;
}

/* static */
int
Script :: getLevel(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getLevel());
    return 1;
}

/* static */
int
Script :: getProfession(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getProfession());
    return 1;
}
