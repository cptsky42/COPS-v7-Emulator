/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "script.h"
#include "client.h"
#include "player.h"
#include "allmsg.h"
#include "lua.hpp"

/* static */
Script::State* Script::State::sState = nullptr;

/* static */
lua_State&
Script::State :: getState()
{
    static volatile long protect = 0;

    if (sState == nullptr)
    {
        if (1 == atomic_inc(&protect))
        {
            // create the instance
            sState = new State();
        }
        else
        {
            while (sState == nullptr)
                QThread::yieldCurrentThread();
        }
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
    lua_register(state, "getMate", Script::getMate);
    lua_register(state, "getLook", Script::getLook);
    lua_register(state, "getHair", Script::getHair);
    lua_register(state, "getMoney", Script::getMoney);
    lua_register(state, "getCPs", Script::getCPs);
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

    lua_register(state, "gainMoney", Script::gainMoney);
    lua_register(state, "gainCPs", Script::gainCPs);
    lua_register(state, "spendMoney", Script::spendMoney);
    lua_register(state, "spendCPs", Script::spendCPs);

    lua_register(state, "move", Script::move);

    // MsgDialog Lua methods
    lua_register(state, "text", Script::text);
    lua_register(state, "link", Script::link);
    lua_register(state, "pic", Script::pic);
    lua_register(state, "create", Script::create);

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
Script :: getMate(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushstring(aState, player.getMate());
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
Script :: getCPs(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    lua_pushinteger(aState, player.getCPs());
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



/* static */
int
Script :: gainMoney(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    int money = lua_tointeger(aState, 2);

    bool success = player.gainMoney((uint32_t)money, true);

    lua_pushboolean(aState, success);
    return 1;
}

/* static */
int
Script :: gainCPs(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    int cps = lua_tointeger(aState, 2);

    bool success = player.gainCPs((uint32_t)cps, true);

    lua_pushboolean(aState, success);
    return 1;
}

/* static */
int
Script :: spendMoney(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    int money = lua_tointeger(aState, 2);

    bool success = player.spendMoney((uint32_t)money, true);

    lua_pushboolean(aState, success);
    return 1;
}

/* static */
int
Script :: spendCPs(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    int cps = lua_tointeger(aState, 2);

    bool success = player.spendCPs((uint32_t)cps, true);

    lua_pushboolean(aState, success);
    return 1;
}

/* static */
int
Script :: move(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    Player& player = *client.getPlayer();
    ASSERT(&player != nullptr);

    uint32_t mapId = (uint32_t)lua_tointeger(aState, 2);
    uint16_t x = (uint16_t)lua_tointeger(aState, 3);
    uint16_t y = (uint16_t)lua_tointeger(aState, 4);

    bool success = player.move(mapId, x, y);

    lua_pushboolean(aState, success);
    return 1;
}



/* static */
int
Script :: text(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    const char* text = lua_tolstring(aState, 2, nullptr);

    MsgDialog msg(text, 0, 0, MsgDialog::ACTION_TEXT);
    client.send(&msg);

    return 0;
}

/* static */
int
Script :: link(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    const char* text = lua_tolstring(aState, 2, nullptr);
    int idx = lua_tointeger(aState, 3);

    MsgDialog msg(text, 0, idx, MsgDialog::ACTION_LINK);
    client.send(&msg);

    return 0;
}

/* static */
int
Script :: pic(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    int pic = lua_tointeger(aState, 2);

    MsgDialog msg(0, 0, pic, 0, MsgDialog::ACTION_PIC);
    client.send(&msg);

    return 0;
}

/* static */
int
Script :: create(lua_State* aState)
{
    Client& client = *((Client*)lua_tointeger(aState, 1));
    ASSERT(&client != nullptr);

    MsgDialog msg(0xFF, MsgDialog::ACTION_CREATE);
    client.send(&msg);

    return 0;
}
