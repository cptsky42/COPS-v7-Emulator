/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgconnect.h"
#include "client.h"
#include "player.h"
#include "database.h"
#include "world.h"
#include "msgtalk.h"
#include "msguserinfo.h"
#include "msguserattrib.h"
#include "msgiteminfo.h"
#include "msgtick.h"
#include "msgdate.h"

MsgConnect :: MsgConnect(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgConnect :: ~MsgConnect()
{

}

void
MsgConnect :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);

    static const Database& db = Database::getInstance(); // singleton

    Client& client = *aClient;
    Client::Status status = client.getStatus();

    // set the account UID
    client.setAccountID(mInfo->AccountUID);

    switch (status)
    {
        case Client::NOT_AUTHENTICATED: // Sent to the AccServer
        {
            fprintf(stderr, "MsgConnect::process() [AccServer]\n");
            client.disconnect();
            break;
        }
        case Client::NORMAL: // Sent to the MsgServer
        {
            if (!IS_SUCCESS(db.getPlayerInfo(client)))
            {
                client.disconnect();
                break;
            }

            Msg* msg = nullptr;
            if (client.getPlayer() == nullptr)
            {
                msg = new MsgTalk(STR_SYSTEM_NAME, STR_ALLUSERS_NAME, STR_REPLY_NEW_ROLE, MsgTalk::CHANNEL_ENTRANCE);
                client.send(msg);
                SAFE_DELETE(msg);
            }
            else
            {
                Player& player = *client.getPlayer();
                Player* other = nullptr;
                World& world = World::getInstance();

                // TODO: valid sequence with client ? to avoid rollback..
                if (world.queryPlayer(&other, player.getUID()))
                {
                    world.removePlayer(*other);
                    other->disconnect();
                }

                world.addPlayer(player);

                msg = new MsgTalk(STR_SYSTEM_NAME, STR_ALLUSERS_NAME, STR_REPLY_OK, MsgTalk::CHANNEL_ENTRANCE);
                client.send(msg);
                SAFE_DELETE(msg);

                msg = new MsgDate();
                client.send(msg);
                SAFE_DELETE(msg);

                msg = new MsgUserInfo(player);
                client.send(msg);
                SAFE_DELETE(msg);

                msg = new MsgTalk("SYSTEM", "ALLUSERS", STR_CREATOR_INFO, MsgTalk::CHANNEL_TALK);
                client.send(msg);
                SAFE_DELETE(msg);

                msg = new MsgTalk("SYSTEM", "ALLUSERS", STR_BUILD_INFO, MsgTalk::CHANNEL_TALK);
                client.send(msg);
                SAFE_DELETE(msg);

                player.sendSysMsg("MaxHP: %u, MaxMP: %u, MaxEnergy: %u",
                                  player.getMaxHP(), player.getMaxMP(), player.getMaxEnergy());
                player.sendSysMsg("MinAtk: %d, MaxAtk: %d, Def: %d, MAtk: %d, MDef: %d, Dext: %u",
                                  player.getMinAtk(), player.getMaxAtk(), player.getDefense(), player.getMAtk(),
                                  player.getMDef(), player.getDext());
            }

            break;
        }
        default: // FIXME !
            ASSERT(false);
            break;
    }
}

void
MsgConnect :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->AccountUID = bswap32(info->AccountUID);
    info->Data = bswap32(info->Data);
}
