/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgallot.h"
#include "client.h"
#include "player.h"

MsgAllot :: MsgAllot(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgAllot :: ~MsgAllot()
{

}

void
MsgAllot :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    if (&player == nullptr)
        return;

    uint16_t sum = mInfo->Force + mInfo->Health + mInfo->Soul + mInfo->Dexterity;
    if (player.getAddPoints() < sum)
    {
        LOG(WARN, "[CHEAT] Player %s (client=%p) tried to allot more points !",
            player.getName(), &client);

        player.sendSysMsg(STR_CHEAT);
        return;
    }

    player.allot(mInfo->Force, mInfo->Health, mInfo->Dexterity, mInfo->Soul);
}
