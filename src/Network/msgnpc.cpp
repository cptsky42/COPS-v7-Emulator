/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgnpc.h"
#include "client.h"
#include "player.h"
#include "npc.h"
#include "world.h"
#include <map>

using namespace std;

MsgNpc :: MsgNpc(uint32_t aId, uint32_t aData, uint16_t aType, Event aEvent)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aId, aData, aType, aEvent);
}

MsgNpc :: MsgNpc(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}

MsgNpc :: ~MsgNpc()
{

}

void
MsgNpc :: create(uint32_t aId, uint32_t aData, uint16_t aType, Event aEvent)
{
    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_NPC;

    mInfo->Id = aId;
    mInfo->Data = aData;
    mInfo->Event = (uint16_t)aEvent;
    mInfo->Type = aType;
}

void
MsgNpc :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    if (!player.isAlive())
    {
        player.sendSysMsg(STR_DIE);
        return;
    }

    switch (mInfo->Event)
    {
    case EVENT_BEACTIVED:
        {
            World& world = World::getInstance();
            Npc* npc = nullptr;
            if (world.queryNpc(&npc, mInfo->Id) &&
                player.getMapId() == npc->getMapId())
            {
                LOG(DBG, "Activating npc %d for client %p...",
                    npc->getUID(), &client);

                npc->activateNpc(client, 0);
            }
            break;
        }
//    case EVENT_DELNPC:
//        {
//            //			CNpc* pNpc;
//            //			IRole* pRole = RoleManager()->QuerySet()->GetObj(m_pInfo->id);
//            //			if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pUser->GetMapID() == pNpc->GetMapID())
//            //			{
//            //				if(pNpc->IsDeleted())
//            //					return ;

//            //				if(pNpc->IsDelEnable() && pNpc->IsOwnerOf(pUser))		// °ïÅÉÖù×Ó²»ÄÜÉ¾³ý
//            //				{
//            //					if(pNpc->IsRecycleEnable())
//            //					{
//            //						// Ö»¼ì²éÆÕÍ¨±³°üÊÇ·ñÂú
//            //						if(pUser->IsItemFull(CItem::GetWeight(pNpc->GetInt(NPCDATA_ITEMTYPE)), ID_NONE, ITEMPOSITION_BACKPACK))
//            //						{
//            //							pUser->SendSysMsg(STR_YOUR_BAG_FULL);
//            //							return ;
//            //						}

//            //						CItem* pItem = pNpc->Recycle(pUser->GetID());
//            //						if(pItem)
//            //						{
//            //							pUser->AddItem(pItem, SYNCHRO_TRUE);
//            //							pUser->SendSysMsg(STR_GOT_ITEM, pItem->GetStr(ITEMDATA_NAME));
//            //						}
//            //						else
//            //							pUser->SendSysMsg(STR_ITEM_DAMAGED);
//            //					}
//            //					if(!pNpc->DelNpc())
//            //						pUser->SendSysMsg(STR_DELETE_FAILED);
//            //				}
//            //			}
//            break;
//        }
//    case EVENT_CHANGEPOS:
//        {
//            //			CNpc* pNpc;
//            //			IRole* pRole = RoleManager()->QuerySet()->GetObj(m_pInfo->id);
//            //			if (pRole && pRole->QueryObj(OBJ_NPC, IPP_OF(pNpc)) && pUser->GetMapID() == pNpc->GetMapID())
//            //			{
//            //				int	nLook = ::MaskLook(m_pInfo->usType);
//            //				if(::MaskLook(pNpc->GetLookFace()) == nLook)
//            //					pNpc->ChangeDir(nLook);
//            //				pNpc->TransPos(m_pInfo->usPosX, m_pInfo->usPosY);		// synchro true
//            //			}
//            break;
//        }
    default:
        {
            fprintf(stdout, "Unknown event[%04u], data=[%d]\n",
                    mInfo->Event, mInfo->Data);
            break;
        }
    }
}

void
MsgNpc :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Id = bswap32(mInfo->Id);
    if (info->Event == EVENT_CHANGEPOS)
    {
        info->PosX = bswap16(info->PosX);
        info->PosY = bswap16(info->PosY);
    }
    else
    {
        info->Data = bswap32(info->Data);
    }
    info->Event = bswap16(info->Event);
    info->Type = bswap16(info->Type);
}
