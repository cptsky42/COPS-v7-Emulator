/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "player.h"
#include "magic.h"
#include "weaponskill.h"
#include "itemtask.h"
#include "world.h"
#include "mapmanager.h"
#include "allmsg.h"
#include <stdarg.h>
#include <math.h>
#include <map>
#include <algorithm>

using namespace std;

Player :: Player(Client& aClient, uint32_t aUID)
    : AdvancedEntity(aUID), mClient(aClient),
      mMate("None"),
      mHair(0),
      mProfession(0), mMetempsychosis(0), mExp(0),
      mForce(0), mDexterity(0), mHealth(0), mSoul(0), mAddPoints(0),
      mCurMP(0),
      mMoney(0), mCPs(0), mPkPoints(0), mVirtue(0),
      mEnergy(0), mXP(0),
      mPrevMap(0), mPrevX(0), mPrevY(0),
      mPkMode(Player::PKMODE_FREE)
{
    // no equipment...
    for (uint8_t pos = 0; pos < Item::MAX_EQUIPMENT; ++pos)
        mEquipment[pos] = nullptr;

    mMsgCount = 0;
    mFirstClientTick = 0;
    mLastClientTick = 0;
    mLastRcvClientTick = 0;
    mFirstServerTick = 0;
    mLastServerTick = 0;

    mLastCoolShow = 0;
    mLastAddEnergy.start(Player::ADD_ENERGY_SECS);
}

Player :: ~Player()
{
    static const MapManager& mgr = MapManager::getInstance();
    GameMap* map = mgr.getMap(mMapId);

    if (map != nullptr)
        map->leaveRoom(*this);

    deleteAllItem();
    deleteAllWeaponSkill();
    deleteAllMagicSkill();
}

int32_t
Player :: getMinAtk() const
{
    double atk = mForce;

    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
            atk += pos != Item::POS_LWEAPON ? equip->getMinAtk() : equip->getMinAtk() / 2;
    }

    return max(0.0, atk);
}

int32_t
Player :: getMaxAtk() const
{
    double atk = mForce;

    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
            atk += pos != Item::POS_LWEAPON ? equip->getMaxAtk() : equip->getMaxAtk() / 2;
    }

    return max(0.0, atk);
}

int32_t
Player :: getDefense() const
{
    double def = 0.0;

    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
            def += equip->getDefense();
    }

    return (int32_t)def;
}

int32_t
Player :: getMAtk() const
{
    double atk = 0.0;

    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
            atk += equip->getMagicAtk();
    }

    return max(0.0, atk);
}

int32_t
Player :: getMDef() const
{
    double def = 0.0;

    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
            def += equip->getMagicDef();
    }

    return (int32_t)def;
}

int32_t
Player :: getAdditionAtk() const
{
    int32_t atk = 0;
    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
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
Player :: getAdditionDef() const
{
    int32_t def = 0;
    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
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
Player :: getAdditionMAtk() const
{
    int32_t atk = 0;
    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
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
Player :: getAdditionMDef() const
{
    int32_t def = 0;
    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
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
Player :: getDext() const
{
    uint8_t dext = mDexterity;

    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
            dext += equip->getDexterity();
    }

    return dext;
}

uint16_t
Player :: getMaxHP()  const
{
    int32_t life = (mForce * 3) + (mDexterity * 3) + (mHealth * 24) + (mSoul * 3);

    switch (mProfession)
    {
        case 11:
            life = (int32_t)(life * 1.05);
            break;
        case 12:
            life = (int32_t)(life * 1.08);
            break;
        case 13:
            life = (int32_t)(life * 1.10);
            break;
        case 14:
            life = (int32_t)(life * 1.12);
            break;
        case 15:
            life = (int32_t)(life * 1.15);
            break;
    }

    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
        {
            life += equip->getLife();
            life += equip->getEnchant();
        }
    }

    return max(0, life);
}

uint16_t
Player :: getMaxMP() const
{
    int32_t mana = (mSoul * 5);

    if (mProfession > 100 && mProfession < 200)
        mana += (int32_t)(mana * (mProfession % 10));

    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
            mana += equip->getMana();
    }

    return max(0, mana);
}

uint8_t
Player :: getMaxXP() const
{
    return 100;
}

uint8_t
Player :: getMaxEnergy() const
{
    int32_t energy = 100;
    // TODO blessing

    return energy;
}

void
Player :: enterMap()
{
    static const MapManager& mgr = MapManager::getInstance();
    GameMap* map = mgr.getMap(mMapId);

    if (map != nullptr)
    {
        //    SendLight();
        //    int	nKeepSecs = 0;		// keep light

        Msg* msg = nullptr;

        msg = new MsgAction(this, map->getLight(), MsgAction::ACTION_MAP_ARGB);
        send(msg);
        SAFE_DELETE(msg);

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

void
Player :: leaveMap()
{
    static const MapManager& mgr = MapManager::getInstance();
    GameMap* map = mgr.getMap(mMapId);

    if (map != nullptr)
        map->leaveRoom(*this);

    MsgAction msg(this, 0, MsgAction::ACTION_LEAVE_MAP);
    broadcastRoomMsg(&msg, false);

    clearBroadcastSet();
}

bool
Player :: move(uint32_t aMapId, uint16_t aX, uint16_t aY)
{
    static const MapManager& mgr = MapManager::getInstance();
    GameMap* map = mgr.getMap(aMapId);

    if (map != nullptr)
    {
        leaveMap();

        // TODO detach status STATUS_HIDDEN

        // StandRestart()
        // map->ChangeRegion()

        mPrevMap = mMapId;
        mPrevX = mPosX;
        mPrevY = mPosY;

        mMapId = aMapId;
        mPosX = aX;
        mPosY = aY;

        mPose = POSE_STANDBY;
        // IsInBattle = false, MagicIntone = false, Mining = false
        // ProcessAfterMove()

        MsgAction msg(this, mMapId, MsgAction::ACTION_FLY_MAP);
        send(&msg);

        enterMap();
        return true;
    }

    return false;
}

bool
Player :: move(uint16_t aX, uint16_t aY, uint8_t aDir)
{
    static const MapManager& mgr = MapManager::getInstance();
    GameMap* map = mgr.getMap(mMapId);

    if (map != nullptr)
    {
        if (!map->isValidPoint(aX, aY) || !map->isStandEnable(aX, aY))
        {
            sendSysMsg(STR_INVALID_COORDINATE);
            kickBack();
            return false;
        }

        // the maximum elevation difference (between the character's initial
        // position and the check tile's position) is 210
        if (map->getFloorAlt(aX, aY) - map->getFloorAlt(mPosX, mPosY) > 210)
        {
            // TODO ? Jail with wall jump hack ?
            kickBack();
            return false;
        }

        // TODO detach status STATUS_HIDDEN

        // StandRestart()
        // map->ChangeRegion()

        mPrevX = mPosX;
        mPrevY = mPosY;

        mPosX = aX;
        mPosY = aY;
        mDirection = aDir;

        mPose = POSE_STANDBY;

        updateBroadcastSet();

        // IsInBattle = false, MagicIntone = false, Mining = false
        // ProcessAfterMove()

        return true;
    }

    return false;
}

void
Player :: kickBack()
{
    move(mPrevX, mPrevY, mDirection);

    MsgAction msg(this, mMapId, MsgAction::ACTION_FLY_MAP);
    broadcastRoomMsg(&msg, true);
}

void
Player :: sendShow(const Player& aPlayer) const
{
    MsgPlayer msg(*this);
    aPlayer.send(&msg);

    // TODO KO number ?
}

void
Player :: timerElapsed()
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

    if (isAlive())
    {
        if (true) //TODO if (!isWing())
        {
            if (getEnergy() < getMaxEnergy() &&
                mLastAddEnergy.toNextTime())
            {
                if (Player::POSE_STANDBY == getPose())
                    addAttrib(MsgUserAttrib::USER_ATTRIB_ENERGY, ADD_ENERGY_STAND, true, false);
                else if (Player::POSE_SITDOWN == getPose())
                    addAttrib(MsgUserAttrib::USER_ATTRIB_ENERGY, ADD_ENERGY_SIT, true, false);
            }
        }
    }

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
}

bool
Player :: gainMoney(uint32_t aMoney, bool aSend)
{
    uint64_t money = aMoney + mMoney;
    mMoney = (uint32_t)min(money, (uint64_t)UINT32_MAX);

    if (aSend)
    {
        MsgUserAttrib msg(this, mMoney, MsgUserAttrib::USER_ATTRIB_MONEY);
        send(&msg);
    }

    return money <= UINT32_MAX;
}

bool
Player :: gainCPs(uint32_t aCPs, bool aSend)
{
    uint64_t cps = aCPs + mCPs;
    mCPs = (uint32_t)min(cps, (uint64_t)UINT32_MAX);

    if (aSend)
    {
        MsgUserAttrib msg(this, mCPs, MsgUserAttrib::USER_ATTRIB_CPS);
        send(&msg);
    }

    return cps <= UINT32_MAX;
}

bool
Player :: spendMoney(uint32_t aMoney, bool aSend)
{
    int64_t money = mMoney - aMoney;
    mMoney = (uint32_t)max((int64_t)0, money);

    if (aSend)
    {
        MsgUserAttrib msg(this, mMoney, MsgUserAttrib::USER_ATTRIB_MONEY);
        send(&msg);
    }

    return money > 0;
}

bool
Player :: spendCPs(uint32_t aCPs, bool aSend)
{
    int64_t cps = mCPs - aCPs;
    mCPs = (uint32_t)max((int64_t)0, cps);

    if (aSend)
    {
        MsgUserAttrib msg(this, mCPs, MsgUserAttrib::USER_ATTRIB_CPS);
        send(&msg);
    }

    return cps > 0;
}

bool
Player :: addAttrib(MsgUserAttrib::UserAttrType aType, int64_t aData,
                    bool aSend, bool aBroadcast)
{
    if (aData == 0)
        return false;

    MsgUserAttrib* msg = nullptr;

    switch (aType)
    {
        case MsgUserAttrib::USER_ATTRIB_LEV:
            {
                // TODO MAX_LEVEL
                int64_t lvl = max((int64_t)0, min((int64_t)135, getLevel() + aData));
                mLevel = (uint8_t)lvl;

                msg = new MsgUserAttrib(this, getLevel(), MsgUserAttrib::USER_ATTRIB_LEV);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_LIFE:
            {
                int64_t life = max((int64_t)0, min((int64_t)getMaxHP(), getCurHP() + aData));
                mCurHP = (uint16_t)life;

                msg = new MsgUserAttrib(this, getCurHP(), MsgUserAttrib::USER_ATTRIB_LIFE);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_MANA:
            {
                int64_t mana = max((int64_t)0, min((int64_t)getMaxMP(), getCurMP() + aData));
                mCurMP = (uint16_t)mana;

                msg = new MsgUserAttrib(this, getCurMP(), MsgUserAttrib::USER_ATTRIB_MANA);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_EXP:
            {
                mExp += aData;
                msg = new MsgUserAttrib(this, getExp(), MsgUserAttrib::USER_ATTRIB_EXP);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_PK:
            {
                int64_t pkpoints = max((int64_t)0, min((int64_t)INT16_MAX, mPkPoints + aData));
                mPkPoints = (int16_t)pkpoints;

                msg = new MsgUserAttrib(this, getPkPoints(), MsgUserAttrib::USER_ATTRIB_PK);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_ADD_POINT:
            {
                if (aData < 0)
                    return false;

                mAddPoints += aData;
                msg = new MsgUserAttrib(this, getAddPoints(), MsgUserAttrib::USER_ATTRIB_ADD_POINT);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_SOUL:
            {
                int64_t soul = max((int64_t)0, getSoul() + aData);
                mSoul = (uint16_t)soul;

                msg = new MsgUserAttrib(this, getSoul(), MsgUserAttrib::USER_ATTRIB_SOUL);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_HEALTH:
            {
                int64_t health = max((int64_t)0, getHealth() + aData);
                mHealth = (uint16_t)health;

                msg = new MsgUserAttrib(this, getHealth(), MsgUserAttrib::USER_ATTRIB_HEALTH);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_FORCE:
            {
                int64_t force = max((int64_t)0, getForce() + aData);
                mForce = (uint16_t)force;

                msg = new MsgUserAttrib(this, getForce(), MsgUserAttrib::USER_ATTRIB_FORCE);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_DEXTERITY:
            {
                int64_t dexterity = max((int64_t)0, getDexterity() + aData);
                mDexterity = (uint16_t)dexterity;

                msg = new MsgUserAttrib(this, getDexterity(), MsgUserAttrib::USER_ATTRIB_DEXTERITY);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_XP:
            {
                int64_t xp = max((int64_t)0, min((int64_t)getMaxXP(), getXP() + aData));
                mXP = (uint8_t)xp;

                msg = new MsgUserAttrib(this, getXP(), MsgUserAttrib::USER_ATTRIB_XP);
                break;
            }
        case MsgUserAttrib::USER_ATTRIB_ENERGY:
            {
                int64_t energy = max((int64_t)0, min((int64_t)getMaxEnergy(), getEnergy() + aData));
                mEnergy = (uint8_t)energy;

                msg = new MsgUserAttrib(this, getEnergy(), MsgUserAttrib::USER_ATTRIB_ENERGY);
                break;
            }
        default:
            ASSERT(false);
            return false;
    }

    // TODO ? Save ?

    if (aSend)
        send(msg);

    if (aBroadcast)
        broadcastRoomMsg(msg, false);

    SAFE_DELETE(msg);
    return true;
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

Item*
Player :: getItem(uint32_t aUID) const
{
    Item* item = nullptr;

    mInventoryMutex.lock();
    map<uint32_t, Item*>::const_iterator it;
    if ((it = mInventory.find(aUID)) != mInventory.end())
    {
        item = it->second;
    }
    mInventoryMutex.unlock();

    return item;
}

bool
Player :: canEquipItem(const Item& aItem, Item::Position aPosition) const
{
    if (aItem.isTaskItem())
        return false;

    if (aItem.getReqLevel() > getLevel())
        return false;

    if (aItem.isNeedIdent())
        return false;

    if (aItem.getReqSex() != 0 && (aItem.getReqSex() & (1 << getSex())) == 0)
        return false;

    // rebirth
    if (getMetempsychosis() > 0 && getLevel() >= 70 && aItem.getReqLevel() <= 70)
        return true;

    if (aItem.getReqProf() != 0)
    {
        uint8_t reqProfSort = (aItem.getReqProf() % 1000) / 10;
        uint8_t reqProfLevel = aItem.getReqProf() % 10;
        uint8_t curProfSort = (getProfession() % 1000) / 10;
        uint8_t curProfLevel (getProfession() % 10);

        if (reqProfSort == 19) // taoist
        {
            if (curProfSort < 10)
                return false;
            else
            {
                if (Item::POS_LWEAPON == aPosition)
                    return false;
            }
        }
        else
        {
            if (reqProfSort != curProfSort)
                return false;
        }

        if (reqProfLevel > curProfLevel)
            return false;
    }

    // TODO check for weapon skill
    //if (aItem.getReqWeaponSkill() != 0 && aItem.isWeapon() )

    if (aItem.getReqForce() > getForce() ||
        aItem.getReqSpeed() > getDexterity() ||
        aItem.getReqHealth() > getHealth() ||
        aItem.getReqSoul() > getSoul())
        return false;

    return true;
}

bool
Player :: useItem(Item& aItem, Item::Position aPosition, bool aSend)
{
    ASSERT_ERR(&aItem != nullptr, false);

    bool success = false;

    if (aItem.isActionItem())
    {
        if (aItem.getTask() != nullptr)
        {
            success = ERROR_SUCCESS == aItem.getTask()->execute(mClient, 0);
            // TODO ? erase Item ?
        }
    }
    else if (aItem.isEquipEnable())
    {
        success = equipItem(aItem, aPosition, aSend);
    }
    else if (aItem.isEatEnable())
    {
        int16_t addLife = aItem.getLife();
        int16_t addMana = aItem.getMana();

        if (eraseItem(aItem.getUID(), aSend))
        {
            addAttrib(MsgUserAttrib::USER_ATTRIB_LIFE, addLife, true, true);
            // TODO broadcast team life...

            addAttrib(MsgUserAttrib::USER_ATTRIB_LIFE, addMana, true, false);

            success = true;
        }
    }

    return success;
}

bool
Player :: equipItem(Item& aItem, Item::Position aPosition, bool aSend)
{
    ASSERT_ERR(&aItem != nullptr, false);

    bool success = false;

    // TODO AbortMagic().

    if (canEquipItem(aItem, aPosition))
    {
        // TODO if (isWing()) return false

        // TODO simplify the code
        switch (aPosition)
        {
            case Item::POS_ARMET:
                {
                    if (aItem.isHelmet())
                    {
                        unequipOnly(aPosition);

                        mEquipment[aPosition] = &aItem;
                        aItem.setPosition(aPosition);

                        success = true;
                    }
                    break;
                }
            case Item::POS_NECKLACE:
                {
                    if (aItem.isNecklace())
                    {
                        unequipOnly(aPosition);

                        mEquipment[aPosition] = &aItem;
                        aItem.setPosition(aPosition);

                        success = true;
                    }
                    break;
                }
            case Item::POS_ARMOR:
                {
                    if (aItem.isArmor())
                    {
                        unequipOnly(aPosition);

                        mEquipment[aPosition] = &aItem;
                        aItem.setPosition(aPosition);

                        success = true;
                    }
                    break;
                }
            case Item::POS_RWEAPON:
                {
                    if (aItem.isHoldEnable())
                    {
                        if (aItem.is2HWeapon())
                        {
                            if (false) // TODO !(mWepR && mWepL && !IsBackPackSpare(2))
                            {
                                unequipOnly(Item::POS_RWEAPON);
                                if (mEquipment[Item::POS_LWEAPON] != nullptr &&
                                    !(mEquipment[Item::POS_LWEAPON]->isArrow() && aItem.isBow()))
                                    unequipOnly(Item::POS_LWEAPON); // LWep not empty and not a arrow (and equiping a bow)

                                mEquipment[aPosition] = &aItem;
                                aItem.setPosition(aPosition);

                                success = true;
                            }
                        }
                        else if (aItem.is1HWeapon())
                        {
                            if (true) // !(mWepL && mWepL->isArrow() && !isBackPackSpare(2))
                            {
                                unequipOnly(Item::POS_RWEAPON);
                                if (mEquipment[Item::POS_LWEAPON] != nullptr &&
                                    mEquipment[Item::POS_LWEAPON]->isArrow())
                                    unequipOnly(Item::POS_LWEAPON);

                                mEquipment[aPosition] = &aItem;
                                aItem.setPosition(aPosition);

                                success = true;
                            }
                        }
                    }
                    break;
                }
            case Item::POS_LWEAPON:
                {
                    if (aItem.isHoldEnable())
                    {
                        if (mEquipment[Item::POS_RWEAPON] != nullptr)
                        {
                            // 1H + 1H || 1H + Shield || Bow + Arrow
                            if ((mEquipment[Item::POS_RWEAPON]->is1HWeapon() && (aItem.is1HWeapon() || aItem.isShield())) ||
                                (mEquipment[Item::POS_RWEAPON]->isBow() && aItem.isArrow()))
                            {
                                unequipOnly(aPosition);

                                mEquipment[aPosition] = &aItem;
                                aItem.setPosition(aPosition);

                                success = true;
                            }
                        }
                    }
                    break;
                }
            case Item::POS_RING:
                {
                    if (aItem.isRing())
                    {
                        unequipOnly(aPosition);

                        mEquipment[aPosition] = &aItem;
                        aItem.setPosition(aPosition);

                        success = true;
                    }
                    break;
                }
            case Item::POS_SHOES:
                {
                    if (aItem.isShoes())
                    {
                        unequipOnly(aPosition);

                        mEquipment[aPosition] = &aItem;
                        aItem.setPosition(aPosition);

                        success = true;
                    }
                    break;
                }
            default:
                ASSERT(false);
                break;
        }
    }

    if (success)
    {
        if (aSend)
        {
            MsgItem msg(aItem.getUID(), aPosition, MsgItem::ACTION_EQUIP);
            send(&msg);

            if (Item::POS_ARMET == aPosition ||
                Item::POS_ARMOR == aPosition ||
                Item::POS_RWEAPON == aPosition ||
                Item::POS_LWEAPON == aPosition)
            {
                MsgPlayer msgPlayer(*this);
                broadcastRoomMsg(&msgPlayer, false);
            }
        }

        // TODO calcFightRate();
    }

    return success;
}

void
Player :: unequipOnly(Item::Position aPosition)
{
    Item* equip = getEquipByPos(aPosition);
    if (equip == nullptr)
        return;

    // TODO mInventory->isFull()

    if (addItem(equip, true))
        mEquipment[aPosition] = nullptr;
}

bool
Player :: awardItem(const Item::Info& aInfo, bool aSend)
{
    ASSERT_ERR(&aInfo != nullptr, false);

    bool success = true;

    /* TODO ? auto combine ?
    if(bAutoCombine && CItem::IsCountable(nItemType))
    {
        //CItemTypeData* pType = ItemType()->QueryItemType(pInfo->idType);
        //CHECKF(pType);
        if(!CItem::IsNeedIdent(pInfo->nIdent) && pInfo->nAmount < pInfo->nAmountLimit)
        {
            CItem* pItem = FindCombineItem(nItemType);
            if(pItem)
            {
                // check size
                if(pItem->GetInt(ITEMDATA_AMOUNT) + pInfo->nAmount > pInfo->nAmountLimit
                   && m_pPackage->IsPackFull(pItem))
                {
                    if (pItem->IsGhostGem())
                        this->SendSysMsg(STR_GEMBAG_FULL);
                    else
                        this->SendSysMsg(STR_ITEMBAG_FULL);
                    return NULL;
                }

                return CombineNewItem(pItem, pInfo, bSynchro);
            }
        }
    }
    */

    if (mInventory.size() < Player::MAX_INVENTORY_SIZE)
    {
        Item* item = nullptr;
        if (Item::createItem(&item, aInfo, *this))
        {
            if (!addItem(item, aSend))
            {
                LOG(ERROR, "Player::addItem() failed in Player::awardItem().");

                err_t err = item->erase();
                ASSERT(ERROR_SUCCESS == err);

                SAFE_DELETE(item);
                success = false;
            }
        }
        else
        {
            sendSysMsg(STR_FAILED_GENERATE_ITEM);
            success = false;
        }
    }
    else
    {
        sendSysMsg(STR_ITEMBAG_FULL);
        success = false;
    }

    return success;
}

bool
Player :: addItem(Item* aItem, bool aSend)
{
    ASSERT_ERR(aItem != nullptr, false);

    bool success = true;

    mInventoryMutex.lock();
    if (mInventory.size() < Player::MAX_INVENTORY_SIZE)
    {
        if (aItem->getOwner() == nullptr || aItem->getOwner()->getUID() != getUID())
            aItem->setOwner(this);

        if (aItem->getPlayer() == nullptr || aItem->getPlayer()->getUID() != getUID())
            aItem->setPlayer(this);

        map<uint32_t, Item*>::iterator it;
        if ((it = mInventory.find(aItem->getUID())) == mInventory.end())
        {
            mInventory.insert(it, pair<uint32_t, Item*>(aItem->getUID(), aItem));
        }
        else
        {
            // already in the inventory...
            success = false;
        }
    }
    else
    {
        sendSysMsg(STR_ITEMBAG_FULL);
        success = false;
    }
    mInventoryMutex.unlock();

    if (aSend)
    {
        MsgItemInfo msg(*aItem, MsgItemInfo::ACTION_ADD_ITEM);
        send(&msg);
    }

    return success;
}

bool
Player :: eraseItem(uint32_t aUID, bool aSend)
{
    bool success = true;

    mInventoryMutex.lock();
    map<uint32_t, Item*>::iterator it;
    if ((it = mInventory.find(aUID)) != mInventory.end())
    {
        Item* item = it->second;
        ASSERT(item != nullptr);

        err_t err = item->erase();
        ASSERT(ERROR_SUCCESS == err);

        mInventory.erase(it);
        SAFE_DELETE(item);

        if (aSend)
        {
            MsgItem msg(aUID, MsgItem::ACTION_DROP);
            send(&msg);
        }
    }
    else
        success = false;
    mInventoryMutex.unlock();

    return success;
}

void
Player :: sendItemSet() const
{
    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
        {
            MsgItemInfo msg(*equip, MsgItemInfo::ACTION_ADD_ITEM);
            send(&msg);
        }
    }

    mInventoryMutex.lock();
    for (map<uint32_t, Item*>::const_iterator
            it = mInventory.begin(), end = mInventory.end();
         it != end; ++it)
    {
        const Item* item = it->second;
        if (item != nullptr)
        {
            MsgItemInfo msg(*item, MsgItemInfo::ACTION_ADD_ITEM);
            send(&msg);
        }
    }
    mInventoryMutex.unlock();
}

void
Player :: saveAllItem() const
{
    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
    {
        Item* equip = mEquipment[pos];
        if (equip != nullptr)
            equip->save();
    }

    mInventoryMutex.lock();
    for (map<uint32_t, Item*>::const_iterator
            it = mInventory.begin(), end = mInventory.end();
         it != end; ++it)
    {
        const Item* item = it->second;
        if (item != nullptr)
            item->save();
    }
    mInventoryMutex.unlock();
}

void
Player :: deleteAllItem()
{
    for (uint8_t pos = 1; pos < Item::MAX_EQUIPMENT; ++pos)
        SAFE_DELETE(mEquipment[pos]);

    mInventoryMutex.lock();
    for (map<uint32_t, Item*>::iterator
            it = mInventory.begin(), end = mInventory.end();
         it != end; ++it)
    {
        Item* item = it->second;
        SAFE_DELETE(item);
    }
    mInventory.clear();
    mInventoryMutex.unlock();
}

void
Player :: sendWeaponSkillSet() const
{
    mWeaponSkillsMutex.lock();
    for (map<uint16_t, WeaponSkill*>::const_iterator
            it = mWeaponSkills.begin(), end = mWeaponSkills.end();
         it != end; ++it)
    {
        const WeaponSkill* skill = it->second;
        if (skill != nullptr)
        {
            MsgWeaponSkill msg(*skill);
            send(&msg);
        }
    }
    mWeaponSkillsMutex.unlock();
}

void
Player :: deleteAllWeaponSkill()
{
    mWeaponSkillsMutex.lock();
    for (map<uint16_t, WeaponSkill*>::iterator
            it = mWeaponSkills.begin(), end = mWeaponSkills.end();
         it != end; ++it)
    {
        WeaponSkill* skill = it->second;
        SAFE_DELETE(skill);
    }
    mWeaponSkills.clear();
    mWeaponSkillsMutex.unlock();
}

void
Player :: sendMagicSkillSet() const
{
    mMagicsMutex.lock();
    for (map<uint16_t, Magic*>::const_iterator
            it = mMagics.begin(), end = mMagics.end();
         it != end; ++it)
    {
        const Magic* magic = it->second;
        if (magic != nullptr)
        {
            MsgMagicInfo msg(*magic);
            send(&msg);
        }
    }
    mMagicsMutex.unlock();
}

void
Player :: deleteAllMagicSkill()
{
    mMagicsMutex.lock();
    for (map<uint16_t, Magic*>::iterator
            it = mMagics.begin(), end = mMagics.end();
         it != end; ++it)
    {
        Magic* magic = it->second;
        SAFE_DELETE(magic);
    }
    mMagics.clear();
    mMagicsMutex.unlock();
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
