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

    mInfo->UniqId = aPlayer.getUID();
    mInfo->Timestamp = 0;
    memset(mInfo->Junk, 0, sizeof(mInfo->Junk));
    mInfo->CheckData = 0;
}

static uint32_t checksum(const char* aName)
{
    if (aName == nullptr || aName[0] == '\0' || strlen(aName) < 4)
        return UINT32_C(0x9D4B5703);
    else
    {
        uint16_t val = ((uint16_t*)aName)[0];
        #if BYTE_ORDER == BIG_ENDIAN
        val = bswap16(val);
        #endif // BYTE_ORDER == BIG_ENDIAN

        return val ^ UINT16_C(0x9823);
    }
}

void
MsgTick :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    uint32_t uid = mInfo->UniqId;
    int32_t time = mInfo->Timestamp ^ mInfo->UniqId;
    uint32_t checkData = mInfo->CheckData;

    if (uid != player.getUID())
    {
        client.disconnect();
        return;
    }

    if (checkData != checksum(player.getName()))
    {
        client.disconnect();
        return;
    }

    player.processTick(time, 1);
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
