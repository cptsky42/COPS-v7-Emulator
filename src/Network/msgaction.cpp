/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgaction.h"
#include "client.h"
#include "entity.h"
#include "player.h"
#include "database.h"
#include "mapmanager.h"
#include "gamemap.h"
#include "msgplayer.h"

MsgAction :: MsgAction(const Entity* aEntity, int32_t aData, Action aAction)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aEntity, aData, aAction);
}

MsgAction :: MsgAction(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgAction :: ~MsgAction()
{

}

void
MsgAction :: create(const Entity* aEntity, int32_t aData, Action aAction)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_ACTION;

    mInfo->Timestamp = timeGetTime();
    if (aEntity != nullptr)
    {
        mInfo->UniqId = aEntity->getUID();
        mInfo->PosX = aEntity->getPosX();
        mInfo->PosY = aEntity->getPosY();
        mInfo->Direction = aEntity->getDirection();
    }
    else
    {
        mInfo->UniqId = 0;
        mInfo->PosX = 0;
        mInfo->PosY = 0;
        mInfo->Direction = 0;
    }

    mInfo->Data = aData;
    mInfo->Action = aAction;
}

void
MsgAction :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    switch (mInfo->Action)
    {
        case ACTION_CHG_DIR:
            {
                if (player.getUID() != mInfo->UniqId)
                {
                    client.disconnect();
                    return;
                }

                player.setDirection((uint8_t)mInfo->Direction);
                player.broadcastRoomMsg(this, false);
                break;
            }
        case ACTION_EMOTION:
            {
                if (player.getUID() != mInfo->UniqId)
                {
                    client.disconnect();
                    return;
                }

                // TODO: mining = false, intone = false, in battle = false

                player.setPose((uint16_t)mInfo->Data);
                if (AdvancedEntity::POSE_COOL == player.getPose())
                {
                    if (timeGetTime() - player.getLastCoolShow() > 3000) // 3s
                    {
                        // TODO: cool effect
                        if (true)//(isAllNonsuchEquip())
                            mInfo->Data |= (player.getProfession() * 0x00010000 + 0x01000000);
                        else if (true)//(% 10 == 9)
                            mInfo->Data |= (player.getProfession() * 0x010000);

                        player.setLastCoolShow(timeGetTime());
                    }
                }

                player.broadcastRoomMsg(this, true);
                break;
            }
        case ACTION_CHG_MAP:
            {
                if (player.getUID() != mInfo->UniqId)
                {
                    client.disconnect();
                    return;
                }

                const Database& db = Database::getInstance();
                const MapManager& mgr = MapManager::getInstance();
                const GameMap* map = mgr.getMap(player.getMapId());

                if (map != nullptr)
                {
                    uint16_t passageX = (uint16_t)mInfo->Data;
                    uint16_t passageY = (uint16_t)(mInfo->Data >> 16);

                    // if requested with a big range, it's a hack, else we consider that it's a lag...
                    if (GameMap::distance(player.getPosX(), player.getPosY(), passageX, passageY) > 5)
                    {
                         // TODO crime, send to jail
                         return;
                    }

                    uint32_t mapId = 0;
                    uint16_t posX = 0, posY = 0;

                    int passageId = map->getPassage(passageX, passageY);
                    if (passageId != -1 &&
                        IS_SUCCESS(db.getPasswayInfo(mapId, posX, posY, player.getMapId(), (uint8_t)passageId)))
                    {
                        player.move(mapId, posX, posY);
                    }
                    else
                    {
                        player.kickBack();
                    }
                }
                else
                {
                    // invalid map...
                    client.disconnect();
                }
                break;
            }
        case ACTION_ENTER_MAP:
            {
                const MapManager& mgr = MapManager::getInstance();
                const GameMap* map = mgr.getMap(player.getMapId());

                if (map != nullptr)
                {
                    mInfo->PosX = player.getPosX();
                    mInfo->PosY = player.getPosY();
                    mInfo->Data = map->getDocID();
                    mInfo->Direction = player.getDirection();
                    client.send(this);

                    player.enterMap();
                }
                else
                {
                    // invalid map...
                    client.disconnect();
                }
                break;
            }
        case ACTION_GET_ITEM_SET:
            {
                if (player.getUID() != mInfo->UniqId)
                {
                    client.disconnect();
                    return;
                }

                player.sendItemSet();
                client.send(this);
                break;
            }
        case ACTION_GET_GOOD_FRIEND:
            {
                // TODO send friends / enemies

                client.send(this);
                break;
            }
        case ACTION_GET_WEAPON_SKILL_SET:
            {
                if (player.getUID() != mInfo->UniqId)
                {
                    client.disconnect();
                    return;
                }

                player.sendWeaponSkillSet();
                client.send(this);
                break;
            }
        case ACTION_GET_MAGIC_SET:
            {
                if (player.getUID() != mInfo->UniqId)
                {
                    client.disconnect();
                    return;
                }

                player.sendMagicSkillSet();
                client.send(this);
                break;
            }
        case ACTION_SET_PKMODE:
            {
                if (player.getUID() != mInfo->UniqId)
                {
                    client.disconnect();
                    return;
                }

                const char* msg = nullptr;
                switch ((Player::PkMode)mInfo->Data)
                {
                    case Player::PKMODE_FREE:
                        {
                            msg = STR_FREE_PK_MODE;
                            break;
                        }
                    case Player::PKMODE_SAFE:
                        {
                            msg = STR_SAFE_PK_MODE;
                            break;
                        }
                    case Player::PKMODE_TEAM:
                        {
                            msg = STR_TEAM_PK_MODE;
                            break;
                        }
                    case Player::PKMODE_ARRESTMENT:
                        {
                            msg = STR_ARRESTMENT_PK_MODE;
                            break;
                        }
                    default:
                        {
                            client.disconnect();
                            return;
                        }
                }

                player.setPkMode((Player::PkMode)mInfo->Data);
                //role. isinbattle = false

                client.send(this);
                player.sendSysMsg(msg);
                break;
            }
        case ACTION_GET_SYN_ATTR:
            {
                // TODO send syn attributes

                client.send(this);
                break;
            }
        case ACTION_DESTROY_BOOTH:
            {
                // TODO: Implement booths
                break;
            }
        case ACTION_JUMP:
            {
                if (player.getUID() != mInfo->UniqId)
                {
                    client.disconnect();
                    return;
                }

                static const MapManager& mgr = MapManager::getInstance(); // singleton
                const GameMap* map = mgr.getMap(player.getMapId());

                if (player.getPosX() != mInfo->PosX || player.getPosY() != mInfo->PosY)
                {
                    player.kickBack();
                    return;
                }

                if (!player.isAlive())
                {
                    player.sendSysMsg(STR_DIE);
                    player.kickBack();
                    return;
                }

                if (map != nullptr)
                {
                    uint16_t newX = (uint16_t)mInfo->Data;
                    uint16_t newY = (uint16_t)(mInfo->Data >> 16);
                    uint8_t dir = (uint8_t)mInfo->Direction;

                    // if requested with a big range, it's a hack, else we consider that it's a lag...
                    if (GameMap::distance(player.getPosX(), player.getPosY(), newX, newY) > 17)
                    {
                         // TODO crime, send to jail
                         return;
                    }

                    player.send(this); // send back
                    if (player.move(newX, newY, dir))
                    {
                        // broadcast the message to everyone
                        player.broadcastRoomMsg(this, false);
                    }
                }

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
MsgAction :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Timestamp = bswap32(info->Timestamp);
    info->UniqId = bswap32(info->UniqId);
    info->Data = bswap32(info->Data);
    info->PosX = bswap16(info->PosX);
    info->PosY = bswap16(info->PosY);
    info->Direction = bswap16(info->Direction);
    info->Action = bswap16(info->Action);
}
