/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "player.h"
#include "world.h"
#include "mapmanager.h"
#include "msgtalk.h"
#include "msgaction.h"
#include "msgtick.h"
#include "msgplayer.h"
#include <stdarg.h>
#include <math.h>
#include <map>
#include <algorithm>

#include "npc.h"
#include "npctask.h"

using namespace std;

Player :: Player(Client& aClient, uint32_t aUID)
    : AdvancedEntity(aUID), mClient(aClient)
{
    mName = "Unknown";
    mMate = "None";
    mLook = 0;
    mHair = 0;

    mMoney = 0;

    mProfession = 0;
    mLevel = 0;
    mExp = 0;
    mMetempsychosis = 0;

    mForce = 0;
    mDexterity = 0;
    mHealth = 0;
    mSoul = 0;
    mAddPoints = 0;

    mCurHP = 0;
    mCurMP = 0;

    mPkPoints = 0;
    mVirtue = 0;

    mMapId = 0;
    mPosX = 0;
    mPosY = 0;
    mDirection = 1;

    mPrevMap = 0;
    mPrevX = 0;
    mPrevY = 0;


    mMsgCount = 0;
    mFirstClientTick = 0;
    mLastClientTick = 0;
    mLastRcvClientTick = 0;
    mFirstServerTick = 0;
    mLastServerTick = 0;
}

Player :: ~Player()
{

}

/// TODO real...
#define MAX_EQUIPMENT 0

int32_t
Player :: getMinAtk()
{
    double atk = 0.5;
    switch (mProfession)
    {
        case PROFESSION_MAGE:
            atk += mForce * 0.5;
            break;
        case PROFESSION_WARRIOR:
            atk += mForce * 1.0;
            break;
        case PROFESSION_ARCHER:
            atk += mDexterity * 0.5;
            break;
        default:
            ASSERT(false);
            break;
    }

    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        // fDef += m_pEquipment[i]->GetMinAttack();
    }

    return max(0.0, atk);
}

int32_t
Player :: getMaxAtk()
{
    double atk = 0.5;
    switch (mProfession)
    {
        case PROFESSION_MAGE:
            atk += mForce * 0.5;
            break;
        case PROFESSION_WARRIOR:
            atk += mForce * 1.0;
            break;
        case PROFESSION_ARCHER:
            atk += mDexterity * 0.5;
            break;
        default:
            ASSERT(false);
            break;
    }

    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        // fDef += m_pEquipment[i]->GetMaxAttack();
    }

    return max(0.0, atk);
}

int32_t
Player :: getDefense()
{
    double def = 0.5;
    def += mHealth * 0.5;

    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        // fDef += m_pEquipment[i]->GetDefense();
    }

    return (int32_t)def;
}

int32_t
Player :: getMAtk()
{
    double atk = 0.5;
    if (PROFESSION_MAGE == mProfession)
    {
        atk += mSoul * 0.5;
    }

    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        // fDef += m_pEquipment[i]->GetMaxMagicAttack();
    }

    return max(0.0, atk);
}

int32_t
Player :: getMDef()
{
    double def = 0.5;
    switch (mProfession)
    {
        case PROFESSION_WARRIOR:
            def += mHealth * 0.5;
            break;
        case PROFESSION_ARCHER:
        case PROFESSION_MAGE:
            def += mSoul * 0.5;
            break;
        default:
            ASSERT(false);
            break;
    }

    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        // fDef += m_pEquipment[i]->GetMagicDefense();
    }

    return (int32_t)def;
}

int32_t
Player :: getAdditionAtk()
{
    int32_t atk = 0;
    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        //		CItem* pItem = m_pEquipment[i];
        //		if(pItem && !pItem->TestStatus(_ITEM_STATUS_MAGIC_ADD))
        //		{
        //			if ( i + 1 == ITEMPOSITION_SPRITE )
        //			{
        //				if ( pItem->GetProfessionRequired() == _SPRITE_ADDITION_PATK )
        //				{
        //					nAttack += pItem->GetAmountLimit() ;
        //				}
        //			}
        //			else if(i+1 == ITEMPOSITION_WEAPONR || i+1 == ITEMPOSITION_RINGR ||  i+1 == ITEMPOSITION_SHOES)
        //				nAttack += m_pEquipment[i]->GetAddition();
        //		}
    }

    return atk;
}

int32_t
Player :: getAdditionDef()
{
    int32_t def = 0;
    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        //		CItem* pItem = m_pEquipment[i];
        //		if(pItem && !pItem->TestStatus(_ITEM_STATUS_MAGIC_ADD))
        //		{
        //			if ( i + 1 == ITEMPOSITION_SPRITE )
        //			{
        //				if ( pItem->GetProfessionRequired() == _SPRITE_ADDITION_PDEF )
        //				{
        //					nDef += pItem->GetAmountLimit() ;
        //				}
        //			}
        //			else if(i+1 == ITEMPOSITION_ARMOR || i+1 == ITEMPOSITION_HELMET
        //					||  i+1 == ITEMPOSITION_NECKLACE || i+1 == ITEMPOSITION_WEAPONL)
        //				nDef += m_pEquipment[i]->GetAddition();
        //		}
    }

    return def;
}

int32_t
Player :: getAdditionMAtk()
{
    int32_t atk = 0;
    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
    //		CItem* pItem = m_pEquipment[i];
    //		if(pItem && pItem->TestStatus(_ITEM_STATUS_MAGIC_ADD))
    //		{
    //			if ( i + 1 == ITEMPOSITION_SPRITE )
    //			{
    //				if ( pItem->GetProfessionRequired() == _SPRITE_ADDITION_MATK )
    //				{
    //					nAttack += pItem->GetAmountLimit() ;
    //				}
    //			}
    //			else if(i+1 == ITEMPOSITION_WEAPONR || i+1 == ITEMPOSITION_RINGR ||  i+1 == ITEMPOSITION_SHOES)
    //				nAttack += m_pEquipment[i]->GetAddition();
    //		}
    }

    return atk;
}

int32_t
Player :: getAdditionMDef()
{
    int32_t def = 0;
    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        //		CItem* pItem = m_pEquipment[i];
        //		if(pItem && pItem->TestStatus(_ITEM_STATUS_MAGIC_ADD))
        //		{
        //			if ( i + 1 == ITEMPOSITION_SPRITE )
        //			{
        //				if ( pItem->GetProfessionRequired() == _SPRITE_ADDITION_MDEF )
        //				{
        //					nDef += pItem->GetAmountLimit() ;
        //				}
        //			}
        //			else if(i+1 == ITEMPOSITION_ARMOR || i+1 == ITEMPOSITION_HELMET
        //						||  i+1 == ITEMPOSITION_NECKLACE || i+1 == ITEMPOSITION_WEAPONL)
        //				nDef += m_pEquipment[i]->GetAddition();
        //		}
    }

    return def;
}

uint8_t
Player :: getDext()
{
    uint8_t dext = mDexterity;

    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        // nDex += m_pEquipment[i]->GetDodge();
    }

    return dext;
}

uint16_t
Player :: getMaxLife()
{
    int32_t life = 30 + ((mLevel - 1) * 3);
    switch (mProfession)
    {
        case PROFESSION_MAGE:
            life += mHealth * 6;
            break;
        case PROFESSION_WARRIOR:
            life += mHealth * 5;
            break;
        case PROFESSION_ARCHER:
            life += mHealth * 7;
            break;
        default:
            ASSERT(false);
            break;
    }

    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        // nLife += m_pEquipment[i]->GetLife();
    }

    return max(0, life);
}

uint16_t
Player :: getMaxMana()
{
    int32_t mana = (mSoul * 5) + ((mLevel - 1) * 3);

    for (uint8_t pos = 0; pos < MAX_EQUIPMENT; ++pos)
    {
        // TODO get equipment
        // nMaxMana += m_pEquipment[i]->GetMana();
    }

    return max(0, mana);
}

uint8_t
Player :: getMaxXP()
{
    return 100;
}

uint8_t
Player :: getMaxEnergy()
{
    int32_t energy = 100;
    switch (mProfession)
    {
        case PROFESSION_MAGE:
            energy += ((mLevel - 1) * 2);
            break;
        case PROFESSION_WARRIOR:
            energy += (mLevel / 10);
            break;
        case PROFESSION_ARCHER:
            energy += ((mLevel / 10) * 2);
            break;
        default:
            ASSERT(false);
            break;
    }

    return energy;
}

uint16_t
Player :: getMaxWeight()
{
    int32_t weight = 0;
    switch (mProfession)
    {
        case PROFESSION_WARRIOR:
            weight += 30 + ((mLevel - 1) * 3);
            break;
        case PROFESSION_MAGE:
        case PROFESSION_ARCHER:
            weight += 20 + ((mLevel - 1) * 3);
            break;
        default:
            ASSERT(false);
            break;
    }

    return weight;
}

void
Player :: enterMap()
{
    const MapManager& mgr = MapManager::getInstance();
    GameMap* map = mgr.getMap(mMapId);

    if (map != nullptr)
    {
        //    SendLight();
        //    int	nKeepSecs = 0;		// keep light

        MsgAction msg(this, map->getLight(), MsgAction::ACTION_MAP_ARGB);
        send(&msg);

        map->enterRoom(*this);
        //		pMap->SendRegionInfo(this);
        map->sendMapInfo(*this);
        updateBroadcastSet(true);

        //	if (pMap && pMap->IsBoothEnable())
        //		DetachStatus((IRole*)this, STATUS_XPFULL);

        //	CRole::AttachStatus(this->QueryRole(), STATUS_PK_PROTECT, 0, CHGMAP_LOCK_SECS);
    }
    else
    {
        // invalid map...
        mClient.disconnect();
        return;
    }
}

bool
Player :: move(uint16_t aX, uint16_t aY, uint8_t aDir)
{
    const MapManager& mgr = MapManager::getInstance();
    GameMap* map = mgr.getMap(mMapId);

    if (map != nullptr)
    {
        if (!map->isValidPoint(aX, aY) || !map->isStandEnable(aX, aY))
        {
            sendSysMsg(STR_INVALID_COORDINATE);
            //kickBack(); // TODO
            return false;
        }

        // detach status STATUS_HIDDEN //TODO

        // StandRestart()
        // map->ChangeRegion()

        mPrevX = mPosX;
        mPrevY = mPosY;

        mPosX = aX;
        mPosY = aY;
        mDirection = aDir;
        // mAction = Action.StandBy; // TODO

        updateBroadcastSet();

        // IsInBattle = false, MagicIntone = false, Mining = false
        // ProcessAfterMove()
    }

    return true;
}

void
Player :: sendShow(const Player& aPlayer) const
{
    MsgPlayer msg(*this);
    aPlayer.send(&msg);

    // TODO KO number ?
}

void
Player :: timerElapsed(time_t aTime)
{
    int32_t now = timeGetTime();

    //////////////////////////////////////////////////////////////
    ///  MsgTick : Ping / Pong
    //////////////////////////////////////////////////////////////
    if (mFirstServerTick == 0) // run only once per user
    {
        mFirstServerTick = now;
        mLastRcvClientTick = now;

        mLastServerTick = now;
        mServerTicks.push_back(now);

        MsgTick msg(*this);
        send(&msg);
    }
    else
    {
        if (now >= mLastServerTick + 10000) // each 10s
        {
            mLastServerTick = now;
            mServerTicks.push_back(now);

            MsgTick msg(*this);
            send(&msg);
        }
    }

    if (mLastRcvClientTick != 0)
    {
        if (now - mLastRcvClientTick >= 25000) // no feedback after 25s...
        {
            // reject
            mClient.disconnect();
        }
    }


    //	UpdateEquipmentExp();

    //	if(this->GetSyndicate())
    //	{
    //		if(!m_tTimeOnline.IsActive())
    //			m_tTimeOnline.Startup(SYNMEM_TIMEONLINE_INTERVAL);

    //		if(this->m_tTimeOnline.ToNextTime(SYNMEM_TIMEONLINE_INTERVAL))
    //		{
    //			this->QuerySynAttr()->IncTimeOnline(SYNMEM_TIMEONLINE_INTERVAL/60);
    //		}
    //	}

    //	ProcessAutoAttack();

    //	if (m_tXp.ToNextTime())
    //		this->ProcXpVal();

    //	if (m_tSlowHealLife.ToNextTime() && m_setSlowHealUp2Life.Size() > 0)
    //		this->ProcSlowHealLifeUp();

    //	if (m_tSlowHealMana.ToNextTime() && m_setSlowHealUp2Mana.Size() > 0)
    //		this->ProcSlowHealManaUp();

    ////	if (m_tAutoHealLife.ToNextTime())
    ////		this->ProcAutoHealLife();
    ///
    /////	if(IsAlive())		// && !this->QueryTransformation()
    //	{
    ////		if (!this->IsWing())
    //		{
    //			if(QueryStatus(STATUS_PELT))
    //			{
    //				if (m_tEnergy.ToNextTime(DEL_ENERGY_PELT_SECS))
    //				{
    //					if (this->GetEnergy() < DEL_ENERGY_PELT)
    //						CRole::DetachStatus(this->QueryRole(), STATUS_PELT);
    //					else
    //						AddAttrib(_USERATTRIB_ENERGY, -1*DEL_ENERGY_PELT, SYNCHRO_TRUE);
    //				}
    //			}
    //			else if(GetEnergy() < GetMaxEnergy())
    //			{
    //#ifdef	PALED_DEBUG
    //					SendSysMsg("WALK --------------");
    //#endif
    //				if (m_tEnergy.ToNextTime(ADD_ENERGY_STAND_SECS))
    //					AddAttrib(_USERATTRIB_ENERGY, ADD_ENERGY_STAND, SYNCHRO_TRUE);
    //			}
    //		}
    //	}

    //	if (QueryMagic())
    //		QueryMagic()->OnTimer(tCurr);

    //	// bDetachTeamStatus = true ±íÊ¾ÐèÒª½â³ýËùÓÐÓë½á½çÏà¹ØµÄ×´Ì¬
    //	bool	bDetachTeamStatus = false;
    //	bool	bDetachAddExpStatus = false;	// ÊÇ·ñ½â³ýSTATUS_ADD_EXP
    //	CTeam* pTeam = GetTeam();
    //	if (IsAlive() && pTeam)
    //	{
    //		if (pTeam->GetMemberAmount() < TEAM_STATUS_REQ_ROLES)
    //			bDetachTeamStatus = true;

    //		OBJID idLeader = pTeam->GetLeader();
    //		if (idLeader != this->GetID())
    //		{
    //			CUser* pLeader = UserManager()->GetUser(idLeader);
    //			if (!pLeader || !pLeader->IsAlive()
    //				|| this->GetMapID() != pLeader->GetMapID()
    //				|| this->GetDistance(pLeader->QueryMapThing()) > _RANGE_TEAM_STATUS)
    //			{
    //				bDetachTeamStatus = true;
    //			}
    //			if (!pLeader || !pLeader->QueryStatus(STATUS_ADD_EXP))
    //				bDetachAddExpStatus = true;
    //		}
    //	}
    //	else
    //	{
    //		bDetachTeamStatus = true;
    //		bDetachAddExpStatus = true;
    //	}

    //	for(int i = QueryStatusSet()->GetAmount()-1; i >= 0; i--)
    //	{
    //		IStatus* pStatus = QueryStatusSet()->GetObjByIndex(i);
    //		if(pStatus)
    //		{
    //			pStatus->OnTimer(tCurr);
    //			if (!pStatus->IsValid()
    //				|| (bDetachTeamStatus && pStatus->GetID() >= STATUS_TEAM_BEGIN && pStatus->GetID() <= STATUS_TEAM_END)
    //				|| (bDetachAddExpStatus && pStatus->GetID() == STATUS_ADD_EXP))
    //			{
    //				OBJID idStatus = pStatus->GetID();
    //				QueryStatusSet()->DelObj(idStatus);

    //				// superman list
    //				if (idStatus == STATUS_XPFULL)
    //				{
    //					CalcSupermanKills();
    //					CalcFightRate();
    //				}
    //			}
    //		}
    //	}

    //	if (m_tEquipmentDurability.ToNextTime())
    //		this->UpdateEquipmentDurability();

    //	if (m_tPK.ToNextTime())
    //		this->ProcessPkTimer();

    //	if (m_tMine.IsActive() && m_tMine.ToNextTime())
    //	{
    //		this->ProcessMineTimer();
    //		ResetEnergyInc();
    //	}

    //	if (this->GetTeam() && m_tTeam.ToNextTime())
    //		this->ProcessTeamTimer();

    //	if (m_pTransformation && m_tTransformation.TimeOver())
    //	{
    //		AbortTransform();
    //	}

    //	if(m_tCallPet.IsActive() && m_tCallPet.TimeOver())
    //	{
    //		KillCallPet();
    //	}

    //	if(m_pMount)
    //	{
    //		DEADLOOP_CHECK(PID, "m_pMount")
    //		if(!m_tMount.IsActive())
    //			m_tMount.Startup(MOUNT_ADD_INTIMACY_SECS);
    //		else if(m_tMount.ToNextTime())
    //		{
    //			if(m_pMount->GetInt(ITEMDATA_INTIMACY) < MAX_INTIMACY)
    //			{
    //				m_pMount->SetInt(ITEMDATA_INTIMACY, m_pMount->GetInt(ITEMDATA_INTIMACY) + 1, UPDATE_TRUE);
    //				this->SendSysMsg(STR_INC_MOUNT_INTIMACY);

    //				// synchro
    //				CMsgItemInfo	msg;
    //				if (msg.Create(m_pMount, ITEMINFO_UPDATE))
    //					SendMsg(&msg);
    //			}
    //		}
    //	}

    //	if(m_pSprite)
    //	{
    //		DEADLOOP_CHECK(PID, "m_pSprite")
    //		if (!m_tSprite.IsActive())
    //			m_tSprite.Startup(SPRITE_ADD_EXP_SECS);
    //		else if (m_tSprite.ToNextTime())
    //		{
    //			if (m_pSprite->AwardExp(SPRITE_ADD_EXP, this->GetLuck()) > 0)
    //			{
    //				// synchro
    //				CMsgItemInfo	msg;
    //				if (msg.Create(m_pSprite, ITEMINFO_UPDATE))
    //					SendMsg(&msg);
    //			}
    //		}
    //	}

    //	if (IsMoreLeaveWord() && m_tLeaveWord.ToNextTime(5))
    //	{
    //		LeaveWord()->ShowWords(this);
    //	}

    //	if(!m_bStand && m_tStand.TimeOver(KEEP_STAND_MS) && IsAlive())
    //	{
    //		StandTurnon();
    //	}

    //	if (this->GetSynID() != ID_NONE)
    //	{
    //		if (!m_tIncRepute.IsActive())
    //			m_tIncRepute.Startup(SYNREPUTE_SECS);
    //		else if (m_tIncRepute.ToNextTime())
    //		{
    //			CSyndicate* pSyn = this->GetSyndicate();
    //			if (pSyn)
    //			{
    //				if(this->GetSynRank()>RANK_RESERVED)
    //				{
    //					//¼Ó°ïÅÉÉùÍû
    //					pSyn->QueryModify()->AddData(SYNDATA_REPUTE, SYNREPUTE_VAL_PEREACH, false);
    //					//¼Ó°ïÅÉ¹±Ï×¶È
    //					this->QuerySynAttr()->AddProffer(SYNREPUTE_VAL_PROFFER);
    //				}
    //			}
    //		}
    //	}

    //	if (!m_tIncPotential.IsActive())
    //		m_tIncPotential.Startup(INC_POTENTIAL_SECS);
    //	else if (m_tIncPotential.ToNextTime())
    //	{
    //		this->AddAttrib(_USERATTRIB_POTENTIAL, INC_POTENTIAL_NUM, SYNCHRO_TRUE);
    //	}

    //	if (IsAlive() && m_data.GetMaxLifePercent() < 1000)
    //	{
    //		if (!m_tAutoHealMaxLife.IsActive())
    //			m_tAutoHealMaxLife.Startup(AUTOHEAL_MAXLIFE_TIME);
    //		else if (m_tAutoHealMaxLife.ToNextTime())
    //		{
    //			int nMaxLifePercent = m_data.GetMaxLifePercent();
    //			if (nMaxLifePercent >= 500)
    //				nMaxLifePercent += AUTOHEAL_MAXLIFE_FLESH_WOUND;		// ÇáÉË»Ö¸´
    //			else
    //				nMaxLifePercent += AUTOHEAL_MAXLIFE_GBH;				// ÖØÉË»Ö¸´
    //			SetMaxLifePercent(nMaxLifePercent);

    //			if (nMaxLifePercent >= 1000)
    //				m_tAutoHealMaxLife.Clear();
    //		}
    //	}

    //	if (m_tAutoReborn.IsActive() && m_tAutoReborn.IsTimeOut())
    //	{
    //		m_tAutoReborn.Clear();
    //		if (!IsAlive())
    //		{
    //			CItem* pItem = this->GetItemByType(GHOST_GEM_REBORN);
    //			if (pItem)
    //			{
    //				this->SendSysMsg(STR_AUTO_USE_REBORN, pItem->GetStr(ITEMDATA_NAME));
    //				this->EraseItem(pItem->GetID(), true);
    //				this->Reborn(false);	// Ô­µØ¸´»î
    //			}
    //		}
    //	}
}

void
Player :: allot(uint8_t aForce, uint8_t aHealth, uint8_t aDexterity, uint8_t aSoul)
{
    mForce += aForce;
    mHealth += aHealth;
    mDexterity += aDexterity;
    mSoul += aSoul;
    mAddPoints -= (aForce + aHealth + aDexterity + aSoul);
}

void
Player :: sendSysMsg(const char* aFmt, ...) const
{
    va_list args;
    va_start(args, aFmt);

    char buf[MAX_WORDSSIZE];
    vsprintf(buf, aFmt, args);

    MsgTalk msg(STR_SYSTEM_NAME, STR_ALLUSERS_NAME, buf, MsgTalk::CHANNEL_SYSTEM, MsgTalk::COLOR_RED);
    send(&msg);

    va_end(args);
}

void
Player :: processTick(int32_t aClientTime, uint32_t aMsgCount)
{
    if (mMsgCount == 0)
    {
        mMsgCount = aMsgCount;
    }

    if (mMsgCount > aMsgCount || mMsgCount + 16 < aMsgCount) // cheater found !
    {
        LOG(WARN, "[CHEAT] Msg counter of %s (client=%p) is too off. (Should be around %u, got %u).",
            mName.c_str(), &mClient, mMsgCount, aMsgCount);

        mClient.disconnect(); // disconnect the client...
        return;
    }

    if (mFirstClientTick == 0)
    {
        mFirstClientTick = aClientTime;
    }

    if (aClientTime < mLastClientTick) // ridiculous timestamp
    {
        sendSysMsg(STR_CONNECTION_OFF);

        mClient.disconnect(); // disconnect the client...
        return;
    }

    const int32_t CRITICAL_TICK = 500; // 500 ms
    size_t nServerTicks = mServerTicks.size();
    if (mLastClientTick != 0 && nServerTicks >= 2 &&
        aClientTime > mLastClientTick + 10000 + CRITICAL_TICK)
    {
        // suspicious timestamp
        int32_t serverTime = timeGetTime();
        int32_t serverTickInterval = mServerTicks[1] - mServerTicks[0];

        int32_t echoTime = serverTime - mServerTicks[1];
        if (echoTime < aClientTime - mLastClientTick - serverTickInterval)
        {
            LOG(WARN, "[CHEAT] %s (client=%p) has a suspicious timestamp.",
                mName.c_str(), &mClient);
            sendSysMsg(STR_CONNECTION_OFF);

            mClient.disconnect(); // disconnect the client...
            return;
        }
    }

    if (mServerTicks.size() >= 2)
        mServerTicks.pop_back();

    mMsgCount = aMsgCount;
    mLastClientTick = aClientTime;
    mLastRcvClientTick = timeGetTime();
}
