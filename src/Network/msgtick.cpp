/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgtick.h"
#include "client.h"
#include "player.h"
#include "basefunc.h"
#include <string.h>

MsgTick :: MsgTick(const Player& aPlayer)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aPlayer);
}

MsgTick :: MsgTick(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgTick :: ~MsgTick()
{

}

void
MsgTick :: create(const Player& aPlayer)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_TICK;

    // Official servers use the UID, without XORing... The client doesn't care...
    mInfo->UniqId = (aPlayer.getUID() ^ UINT16_C(0x9864));
    mInfo->Timestamp = 0;
    mInfo->CheckData = 0;
}

void
MsgTick :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    uint32_t uid = mInfo->UniqId ^ UINT16_C(0x9864);
    int32_t time = mInfo->Timestamp ^ (mInfo->UniqId * mInfo->UniqId + 9527);

    // getMsgCount() will XOR the count... EoF directly use the value instead of the function
    uint32_t msgCount = mInfo->CheckData;// ^ player.getUID();

    if (uid != player.getUID())
    {
        client.disconnect();
        return;
    }

    player.processTick(time, msgCount);
}

void
MsgTick :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->UniqId = bswap32(info->UniqId);
    info->Timestamp = bswap32(info->Timestamp);
    info->CheckData = bswap32(info->CheckData);
}
