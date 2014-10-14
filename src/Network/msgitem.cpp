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
#include "npc.h"
#include "item.h"
#include "gamemap.h"
#include "world.h"
#include "database.h"
#include "msgiteminfo.h"
#include <algorithm> // max

using namespace std;

MsgItem :: MsgItem(uint32_t aUID, Action aAction)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aUID, 0, 0, aAction);
}

MsgItem :: MsgItem(uint32_t aUID, uint32_t aData, Action aAction)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aUID, aData, 0, aAction);
}

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
MsgItem :: create(uint32_t aUID, uint32_t aData, uint32_t aTargetUID, Action aAction)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_ITEM;

    mInfo->UniqId = aUID;
    mInfo->Data = aData;
    mInfo->Action = (uint16_t)aAction;
    mInfo->Timestamp = timeGetTime();
    mInfo->TargetUID = aTargetUID;
}

void
MsgItem :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    static const World& world = World::getInstance(); // singleton
    static const Database& db = Database::getInstance(); // singleton

    // Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    switch (mInfo->Action)
    {
        case ACTION_BUY:
            {
                Npc* npc = nullptr;
                if (!world.queryNpc(&npc, mInfo->UniqId))
                    return;

                if (!npc->isShopNpc())
                    return;

                if (player.getMapId() != npc->getMapId())
                    return;

                if (GameMap::distance(player.getPosX(), player.getPosY(),
                                      npc->getPosX(), npc->getPosY()) > Entity::CELLS_PER_VIEW)
                    return;

                // TODO ? shop instead of query ?

                const Item::Info* info = nullptr;
                uint8_t moneytype = 0;
                if (IS_SUCCESS(db.getItemFromShop(&info, moneytype, mInfo->UniqId, mInfo->Data)))
                {
                    switch (moneytype)
                    {
                        case 0: // money
                            {
                                uint32_t price = info->Price;
                                if (player.getMoney() < price)
                                {
                                    player.sendSysMsg(STR_NOT_SO_MUCH_MONEY);
                                    return;
                                }

                                if (player.awardItem(*info, true))
                                    ASSERT(player.spendMoney(price, true));
                                break;
                            }
                        case 1: // CPs
                            {
                                uint32_t price = info->CPs;
                                if (player.getCPs() < price)
                                {
                                    player.sendSysMsg(STR_NOT_SO_MUCH_MONEY);
                                    return;
                                }

                                if (player.awardItem(*info, true))
                                    ASSERT(player.spendMoney(price, true));
                                break;
                            }
                        default:
                            ASSERT(false);
                            break;
                    }
                }

                break;
            }
        case ACTION_SELL:
            {
                Npc* npc = nullptr;
                if (!world.queryNpc(&npc, mInfo->UniqId))
                    return;

                if (!npc->isShopNpc())
                    return;

                if (player.getMapId() != npc->getMapId())
                    return;

                if (GameMap::distance(player.getPosX(), player.getPosY(),
                                      npc->getPosX(), npc->getPosY()) > Entity::CELLS_PER_VIEW)
                    return;

                Item* item = player.getItem(mInfo->Data);
                if (item == nullptr)
                {
                    player.sendSysMsg(STR_ITEM_NOT_FOUND);
                    return;
                }

                if (!item->isSellEnable())
                {
                    player.sendSysMsg(STR_NOT_SELL_ENABLE);
                    return;
                }

                uint32_t money = item->getSellPrice();
                if (!player.eraseItem(mInfo->Data, true))
                {
                    player.sendSysMsg(STR_ITEM_INEXIST);
                    return;
                }

                if (!player.gainMoney(money, true))
                    player.sendSysMsg(STR_MONEYBAG_FULL);

                break;
            }
        case ACTION_USE:
            {
                if (mInfo->TargetUID == 0 || mInfo->TargetUID == player.getUID())
                {
                    Item* item = player.getItem(mInfo->UniqId);
                    if (item != nullptr)
                    {
                        if (!player.useItem(*item, (Item::Position)mInfo->Data, true))
                            player.sendSysMsg(STR_UNABLE_USE_ITEM);
                    }
                    else
                        player.sendSysMsg(STR_ITEM_INEXIST);
                }
                else
                {
                    printf("MsgItem::use() -> useItemTo(%u, %u)\n", mInfo->TargetUID, mInfo->UniqId);
                    ASSERT(false);
                }
                break;
            }
        case ACTION_EQUIP:
            {
                ASSERT(false);
                break;
            }
        case ACTION_REPAIR:
            {
                Item* item = player.getItem(mInfo->UniqId);
                if (item == nullptr)
                {
                    player.sendSysMsg(STR_ITEM_NOT_FOUND);
                    return;
                }

                if (!item->isRepairEnable() || item->getAmount() > item->getAmountLimit())
                {
                    player.sendSysMsg(STR_REPAIR_FAILED);
                    return;
                }

                uint32_t money = item->getRepairCost();
                int32_t repair = item->getAmountLimit() - item->getAmount();
                if (money == 0 || repair <= 0)
                    return;

                if (!player.spendMoney(money, true))
                {
                    player.sendSysMsg(STR_REPAIR_NO_MONEY, money);
                    return; // not enough money
                }

                // max durability changed
                if (item->isEquipment())
                {
                    if (item->getAmount() < item->getAmountLimit() / 2)
                    {
                        if (random(0, 100) < 5)
                            item->setAmountLimit(max(1, item->getAmountLimit() - 1));
                    }
                    else if (item->getAmount() < item->getAmountLimit() / 10)
                    {
                        if (random(0, 100) < 10)
                            item->setAmountLimit(max(1, item->getAmountLimit() - 1));
                    }
                    else
                    {
                        if (random(0, 100) < 80)
                            item->setAmountLimit(max(1, item->getAmountLimit() - 1));
                    }
                }
                item->setAmount(item->getAmountLimit(), true);

                MsgItemInfo msg(*item, MsgItemInfo::ACTION_UPDATE);
                player.send(&msg);

                break;
            }
        case ACTION_REPAIRALL:
            {
                ASSERT(false);
                break;
            }
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
    info->Action = bswap32(info->Action);
    info->Timestamp = bswap32(info->Timestamp);
    info->TargetUID = bswap32(info->TargetUID);
}
