/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgitem.h"
#include "client.h"
#include "entity.h"
#include "player.h"

MsgItem :: MsgItem(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgItem :: ~MsgItem()
{

}

void
MsgItem :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    switch (mInfo->Action)
    {
        case ACTION_COMPLETE_TASK:
            {
                player.send(this);
                break;
            }
        default:
            {
                fprintf(stdout, "Unknown action[%04u], data=[%d]\n",
                        mInfo->Action, mInfo->Data);
                break;
            }
    }
}

void
MsgItem :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->UniqId = bswap32(info->UniqId);
    info->Data = bswap32(info->Data);
    info->Action = bswap16(info->Action);
    info->TargetUID = bswap32(info->TargetUID);
}
