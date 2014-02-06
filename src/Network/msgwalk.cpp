/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgwalk.h"
#include "client.h"
#include "player.h"
#include "mapmanager.h"

MsgWalk :: MsgWalk(int32_t aUniqId, uint8_t aDirection, bool aIsRunning)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aUniqId, aDirection, aIsRunning);
}

MsgWalk :: MsgWalk(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgWalk :: ~MsgWalk()
{

}

void
MsgWalk :: create(int32_t aUniqId, uint8_t aDirection, bool aIsRunning)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_WALK;

    mInfo->UniqId = aUniqId;
    mInfo->Direction = aDirection;//(Byte)(((MyMath.Generate(100, 255) % 31) * 8) + Direction);
    mInfo->Mode = !aIsRunning ? 0 : 1;

    mInfo->Padding1 = 0;
}

void
MsgWalk :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    if (player.getUID() != mInfo->UniqId)
    {
        client.disconnect();
        return;
    }

    uint8_t dir = (uint8_t)(mInfo->Direction % 8);
    uint16_t newX = player.getPosX();
    uint16_t newY = player.getPosY();

    switch (dir)
    {
        case 0: { newY += 1; break; }
        case 1: { newX -= 1; newY += 1; break; }
        case 2: { newX -= 1; break; }
        case 3: { newX -= 1; newY -= 1; break; }
        case 4: { newY -= 1; break; }
        case 5: { newX += 1; newY -= 1; break; }
        case 6: { newX += 1; break; }
        case 7: { newX += 1; newY += 1; break; }
    }

    // running... double the step
    if (mInfo->Mode >= 20 && mInfo->Mode <= 27)
    {
        dir = mInfo->Mode - 20;
        switch (dir)
        {
            case 0: { newY += 1; break; }
            case 1: { newX -= 1; newY += 1; break; }
            case 2: { newX -= 1; break; }
            case 3: { newX -= 1; newY -= 1; break; }
            case 4: { newY -= 1; break; }
            case 5: { newX += 1; newY -= 1; break; }
            case 6: { newX += 1; break; }
            case 7: { newX += 1; newY += 1; break; }
        }
    }

    if (!player.isAlive() && !player.isGhost())
    {
        player.sendSysMsg(STR_DIE);
        return;
    }

    player.send(this); // send back...
    if (player.move(newX, newY, dir))
    {
        // broadcast the message to everyone
        player.broadcastRoomMsg(this, false);
    }
}

void
MsgWalk :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->UniqId = bswap32(info->UniqId);
}
