/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "npc.h"
#include "npctask.h"
#include "client.h"
#include "player.h"
#include "msgnpcinfo.h"

Npc :: Npc(uint32_t aUID, const char* aName,
           uint8_t aType, uint16_t aLook,
           uint32_t aMapId, uint16_t aPosX, uint16_t aPosY,
           uint8_t aBase, uint8_t aSort)
    : Entity(aUID)
{
    if (aName != nullptr)
    {
        mName = aName;
    }

    mType = aType;
    mLook = aLook;
    mDirection = aLook % 10;

    mMapId = aMapId;
    mPosX = aPosX;
    mPosY = aPosY;

    mBase = aBase;
    mSort = aSort;

    mTask = nullptr;
}

Npc :: ~Npc()
{

}

bool
Npc :: activateNpc(Client& aClient, int32_t aAction)
{
    ASSERT_ERR(&aClient != nullptr, false);

    bool result = false;

    if (isTaskNpc() /*&& isAlive()*/ && mTask != nullptr)
    {
        const NpcTask& task = queryTask();

        LOG(DBG, "Npc %d is a task npc and the task %d will be executed...",
            mUID, task.getUID());
        result = (task.execute(aClient, 0) == ERROR_SUCCESS);

        if (result)
            aClient.setCurTask(task);
    }

    return result;
}

void
Npc :: sendShow(const Player& aPlayer) const
{
    MsgNpcInfo msg(*this);
    aPlayer.send(&msg);
}
