/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MAGIC_H_
#define _COPS_V7_EMULATOR_MAGIC_H_

#include "common.h"
#include <string>

class Player;
class QSqlQuery;

/**
 * A magic skill of a player.
 */
class Magic
{
public:
    struct Info
    {
    public:
        /** ID of the magic (Type & Level) */
        uint32_t Id;
        /** Type of the magic */
        uint16_t Type;
        /** Sort of the magic. */
        uint8_t Sort;
        /** Whether or not using the magic is a crime. */
        bool Crime;
        /** Whether or not it is a ground magic. */
        bool Ground;
        /** Whether or not it can target multiple entities. */
        bool Multi;
        /** Bit mask for the target type of the magic. */
        uint32_t Target;
        /** Level of the magic. */
        uint16_t Level;
        /** Mana used by the magic. */
        uint16_t UseMP;
        /** Power of the magic / Percentage (if more than 30k). */
        int32_t Power;
        /** Intone time in milliseconds. */
        uint16_t IntoneDuration;
        /** Success rate when using the magic. */
        uint8_t Success;
        // TODO doc
        uint32_t StepSecs;
        // TODO doc
        uint8_t Range;
        // TODO doc
        uint8_t Distance;
        /** Status flag of the magic. */
        uint32_t Status;
        /** Required profession of the magic. */
        uint16_t ReqProf;
        /** Required exp for the next level of the magic. */
        uint32_t ReqExp;
        /** Required player level for the next level of the magic. */
        uint8_t ReqLevel;
        /** Type of the magic skill (e.g. XP skill). */
        uint8_t UseXP; // 0 -> Magic, 1 -> XP Skill, 2 -> Kongfu
        /** Required weapon subtype to use the magic. */
        uint16_t WeaponSubtype;
        // TODO doc
        uint32_t ActiveTimes;
        // TODO doc
        bool AutoActive; // TQ: 0x01: active when kill target
        // TODO doc
        uint32_t FloorAttr;
        /** Whether or not the magic is automatically learned. */
        bool AutoLearn;
        /** Required player level to automatically learn the magic. */
        uint8_t LearnLevel;
        /** Whether or not the right weapon is thrown when using the magic. */
        bool DropWeapon;
        /** Energy used by the magic. */
        uint8_t UseEP;
        // TODO doc
        bool WeaponHit; // TQ: Hit by weapon...
        /** Use the equipped item. */
        uint32_t UseItem;
        /** Next magic to use. */
        Magic::Info* NextMagic; // launch this magic after current magic, use same target and pos
        /** Delay (in milliseconds) before the next magic. */
        uint16_t NextMagicDelay;
        /** Durability used by the magic. */
        uint8_t UseItemNum;
        /** Whether or not the magic is usable in the market. */
        bool UsableInMarket;

    public:
        /* constructor */
        Info()
            : Id(0), Type(0), Sort(0),
              Crime(false), Ground(false), Multi(false), Target(0),
              Level(0), UseMP(0), Power(0), IntoneDuration(0), Success(0), StepSecs(0),
              Range(0), Distance(0), Status(0), ReqProf(0), ReqExp(0), ReqLevel(0),
              UseXP(0), WeaponSubtype(0), ActiveTimes(0), AutoActive(false),
              FloorAttr(0), AutoLearn(false), LearnLevel(0), DropWeapon(false),
              UseEP(0), WeaponHit(false), UseItem(0), NextMagic(nullptr), NextMagicDelay(0),
              UseItemNum(1), UsableInMarket(false)
        {

        }
    };

public:
    /** Position of the SQL data in the result set. */
    enum SqlData_Info
    {
        SQLDATA_INFO_ID = 0,
        SQLDATA_INFO_TYPE,
        SQLDATA_INFO_SORT,
        SQLDATA_INFO_NAME,
        SQLDATA_INFO_CRIME,
        SQLDATA_INFO_GROUND,
        SQLDATA_INFO_MULTI,
        SQLDATA_INFO_TARGET,
        SQLDATA_INFO_LEVEL,
        SQLDATA_INFO_USE_MP,
        SQLDATA_INFO_POWER,
        SQLDATA_INFO_INTONE_DURATION,
        SQLDATA_INFO_SUCCESS,
        SQLDATA_INFO_STEP_SECS,
        SQLDATA_INFO_RANGE,
        SQLDATA_INFO_DISTANCE,
        SQLDATA_INFO_STATUS,
        SQLDATA_INFO_REQ_PROF,
        SQLDATA_INFO_REQ_EXP,
        SQLDATA_INFO_REQ_LEVEL,
        SQLDATA_INFO_USE_XP,
        SQLDATA_INFO_WEAPON_SUBTYPE,
        SQLDATA_INFO_ACTIVE_TIMES,
        SQLDATA_INFO_AUTO_ACTIVE,
        SQLDATA_INFO_FLOOR_ATTR,
        SQLDATA_INFO_AUTO_LEARN,
        SQLDATA_INFO_LEARN_LEVEL,
        SQLDATA_INFO_DROP_WEAPON,
        SQLDATA_INFO_USE_EP,
        SQLDATA_INFO_WEAPON_HIT,
        SQLDATA_INFO_USE_ITEM,
        SQLDATA_INFO_NEXT_MAGIC,
        SQLDATA_INFO_NEXT_MAGIC_DELAY,
        SQLDATA_INFO_USE_ITEM_NUM,
        SQLDATA_INFO_SENDER_ACTION,
        SQLDATA_INFO_UPGRADE_DESC,
        SQLDATA_INFO_DESC,
        SQLDATA_INFO_INTONE_EFFECT,
        SQLDATA_INFO_INTONE_SOUND,
        SQLDATA_INFO_SENDER_EFFECT,
        SQLDATA_INFO_SENDER_SOUND,
        SQLDATA_INFO_TARGET_DELAY,
        SQLDATA_INFO_TARGET_EFFECT,
        SQLDATA_INFO_TARGET_SOUND,
        SQLDATA_INFO_GROUND_EFFECT,
        SQLDATA_INFO_TRACE_EFFECT,
        SQLDATA_INFO_SCREEN_REPRESENT,
        SQLDATA_INFO_USABLE_IN_MARKET,
        SQLDATA_INFO_TARGET_WOUND_DELAY
    };

    /** Position of the SQL data in the result set. */
    enum SqlData
    {
        SQLDATA_ID = 0,
        SQLDATA_OWNER_ID,
        SQLDATA_TYPE,
        SQLDATA_LEVEL,
        SQLDATA_EXP,
        SQLDATA_OLD_LEVEL,
        SQLDATA_UNLEARN
    };

public:
    /**
     * Create a magic object from a SQL query result set.
     *
     * @param[out]  aOutMagic  a pointer that will receive the object
     * @param[in]   aQuery     the SQL result set
     *
     * @retval ERROR_SUCCESS on success
     * @returns An error code otherwise
     */
    static err_t createMagic(Magic** aOutMagic, const QSqlQuery& aQuery);

public:
    /* destructor */
    ~Magic() { }

public:
    /** Get the magic skill's unique ID. */
    uint32_t getUID() const { return mUID; }
    /** Get the magic skill's type. */
    uint16_t getType() const { return mInfo.Type; }

    /** Get the magic skill's owner. */
    const Player& getPlayer() const { return mPlayer; }

    /** Get the magic skill's level. */
    uint16_t getLevel() const { return mInfo.Level; }
    /** Get the magic skill's experience. */
    uint32_t getExp() const { return mExp; }
    /** Get the magic skill's old level. */
    uint8_t getOldLevel() const { return mOldLevel; }
    /** Whether or not the magic skill was unlearned. */
    bool isUnlearn() const { return mIsUnlearn; }

private:
    /* constructor */
    Magic(uint32_t aUID, const Magic::Info& aInfo, const Player& aPlayer)
        : mUID(aUID), mInfo(aInfo), mPlayer(aPlayer),
          mExp(0), mOldLevel(0), mIsUnlearn(false)
    {

    }

private:
    const uint32_t mUID; //!< the unique ID of the magic
    const Magic::Info& mInfo; //!< the generic info of the magic

    const Player& mPlayer; //!< the player owning the magic

    uint32_t mExp; //!< the experience of the magic
    uint8_t mOldLevel; //!< the previous level of the magic (before reborn)
    bool mIsUnlearn; //!< the whether or not the magic was unlearn
};

//enum
//{
//	MAGIC_SORT_ATTACK_SINGLE_HP			= 1,	// 攻击单人，对方去血
//	MAGIC_SORT_RECOVER_SINGLE_HP		= 2,	// 回复单人，对方加血
//	MAGIC_SORT_ATTACK_CROSS_HP			= 3,	// 十字型区域目标去血
//	MAGIC_SORT_ATTACK_SECTOR_HP			= 4,	// 扇形区域目标去血
//	MAGIC_SORT_ATTACK_ROUND_HP			= 5,	// 圆形区域目标去血
//	MAGIC_SORT_ATTACK_SINGLE_STATUS		= 6,	// 攻击单人，对方加状态
//	MAGIC_SORT_RECOVER_SINGLE_STATUS	= 7,	// 回复单人，对方去状态
//	MAGICSORT_SQUARE					= 8,	// 方形攻击
//	MAGICSORT_JUMPATTACK				= 9,	// 跳砍
//	MAGICSORT_RANDOMTRANS				= 10,	// 随机传送
//	MAGICSORT_DISPATCHXP				= 11,	// 怒吼
//	MAGICSORT_COLLIDE					= 12,	// 冲击
//	MAGICSORT_SERIALCUT					= 13,	// 连击
//	MAGICSORT_LINE						= 14,	// 方向魔法
//	MAGICSORT_ATKRANGE					= 15,	// 增加攻击范围
//	MAGICSORT_ATKSTATUS					= 16,	// ~!@#$%^&*()_+
//	MAGICSORT_CALL_TEAMMEMBER			= 17,	// 召唤队员
//	MAGICSORT_RECORDTRANSSPELL			= 18,	// record map position to trans spell.
//	MAGICSORT_TRANSFORM					= 19,	// 变身
//	MAGICSORT_ADDMANA					= 20,	// 加Mana// support self target only.
//	MAGICSORT_LAYTRAP					= 21,
//	MAGICSORT_DANCE						= 22,	// 跳舞
//	MAGICSORT_CALLPET			= 23,			// 召唤兽
//	MAGICSORT_VAMPIRE			= 24,			// 吸血，power is percent award. use for call pet
//	MAGICSORT_INSTEAD			= 25,			// 替身. use for call pet
//	MAGICSORT_DECLIFE			= 26,			// 扣血(当前血的比例)
//	MAGICSORT_GROUNDSTING		= 27,			// 地刺
//	MAGICSORT_REBORN			= 28,			// 复活 -- zlong 2004.5.14
//	MAGICSORT_TEAM_MAGIC		= 29,			// 界结魔法—— 与MAGICSORT_ATTACHSTATUS相同处理，
//	//				这里独立分类只是为了方便客户端识别
//	MAGICSORT_BOMB_LOCKALL		= 30,			// 与MAGICSORT_BOMB处理相同，只是锁定全部目标
//	MAGICSORT_SORB_SOUL			= 31,			// 吸魂魔法
//	MAGICSORT_STEAL				= 32,			// 偷盗，随机从目标身上偷取power个物品
//	MAGICSORT_LINE_PENETRABLE	= 33,			// 攻击者轨迹可以穿人的线性攻击

//	//////////////////////////////////////////////
//	// 新增幻兽魔法类型
//	MAGICSORT_BLAST_THUNDER			= 34,			// 魔雷
//	MAGICSORT_MULTI_ATTACHSTATUS	= 35,			// 群体施加状态
//	MAGICSORT_MULTI_DETACHSTATUS	= 36,			// 群体解除状态
//	MAGICSORT_MULTI_CURE			= 37,			// 群体补血
//	MAGICSORT_STEAL_MONEY			= 38,			// 偷钱
//	MAGICSORT_KO					= 39,			// 必杀技，目标血小于15%自动触发
//	MAGICSORT_ESCAPE				= 40,			// 逃跑/救助
//	//		MAGICSORT_FLASH_ATTACK			= 41,			// 移形换位
//	MAGICSORT_ATTRACK_MONSTER		= 42,			// 吸引怪物
//};

//enum
//{
//	MAGIC_SHAKENORMAL		  = 0x00000001, // 震动效果
//	MAGIC_COLORNORMAL		  = 0x00000002, // 变暗效果
//	MAGIC_SCALENORMAL		  = 0x00000004, // 缩放效果
//};

//enum
//{
//	MAGIC_TARGET_SELF		  = 0x00000001,	// 可以对自己使用
//	MAGIC_TARGET_NONE		  = 0x00000002, // 不需要点目标
//	MAGIC_TARGET_TERRAIN      = 0x00000004, // 对地使用
//	MAGIC_PASSIVE			  = 0x00000008, // 被动魔法，不能使用
//	MAGIC_TARGET_BODY		  = 0x00000010, // 只能对未消失的尸体使用 //其他无效
//};

//enum
//{
//	TYPE_MAGIC		= 0, // 魔法
//	TYPE_XPSKILL	= 1, // xp技
//	TYPE_KONGFU		= 2, // 武功招式
//};

#endif // _COPS_V7_EMULATOR_MAGIC_H_
