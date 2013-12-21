/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_ACTION_H_
#define _COPS_V7_EMULATOR_MSG_ACTION_H_

#include "common.h"
#include "msg.h"

class Entity;

/**
 * Msg sent to the MsgServer to signal small actions like changing the
 * direction, position, jumping, mining, etc.
 *
 * The MsgServer can also send those msgs to the client to signal a small
 * action.
 */
class MsgAction : public Msg
{
public:
    enum Action
    {
        /** No action specified. (Invalid) */
        ACTION_NONE = 0,
        /** Change the direction of an entity */
        ACTION_CHG_DIR = 1,
        actionPosition         = 2,
        actionEmotion			=3,
        actionBroadcastPos		=4,
        actionDivorce			=5,
        actionSelfUnfreeze		=6,
        /** Signal to the server that the client entered in a portal. */
        ACTION_CHG_MAP = 7,
        /** Signal to the client that the player is changing map. */
        ACTION_FLY_MAP = 8,
        actionChgWeather		=9,
        actionFireworks			=10,
        actionDie				=11,
        actionQuitSyn			=12,
        actionWalk				=13,
        /** Set the position of an entity entering in a new map */
        ACTION_ENTER_MAP = 14,
        /** Get the items (client request). The next step is the friends. */
        ACTION_GET_ITEM_SET = 15,
        /** Get the friends (client request). The next step is the weapon skills. */
        ACTION_GET_GOOD_FRIEND = 16,
        actionForward			=17,
        actionLeaveMap			=18,		// ∑˛ŒÒ∆˜->øÕªß∂À,idPlayer
        actionJump				=19,
        actionRun				=20,
        actionEquip				=21,
        actionUnequip			=22,
        /** Signal to the client that the player gained a level */
        ACTION_UP_LEVEL = 23,
        actionXpCLear			=24,
        actionReborn			=25,
        /** Delete the current role. */
        ACTION_DELETE_ROLE = 26,
        /** Get the weapon skills (client request). The next step is the magics. */
        ACTION_GET_WEAPON_SKILL_SET = 27,
        /** Get the magics (client request). The next step is the legion attributes. */
        ACTION_GET_MAGIC_SET = 28,
        /** Set the Pk mode of an entity */
        ACTION_SET_PKMODE = 29,
        /** Get the legion attributes. */
        ACTION_GET_SYN_ATTR = 30,
        actionGhost				=31,
        /** Synchronize the position (the client will send its coords) as answer */
        ACTION_SYNCRHO = 32, // Unknown action[0032], data=[54788624]
        actionQueryFriendInfo	=33,
        actionQueryLeaveWord	=34,
        /** Change the face of the entity */
        ACTION_CHANGE_FACE = 35,
        actionMine				=36, // action 290 ?
        actionTeamMemeberPos	=37,
        actionQueryPlayer		=38,
        actionAbortMagic		=39,
        /** Set the color of the map */
        ACTION_MAP_ARGB = 40,
        actionMapStatus			=41,		// abandon
        actionQueryTeamMember	=42,
        /** Signal to client/server a new booth. */
        ACTION_CREATE_BOOTH = 43, // UID: playerID; PosX,Y: playerpos; unDir:dirofbooth; idTarget:idnpc;
        actionSuspendBooth		=44,		//  ’∆∞⁄ÃØ
        actionResumeBooth		=45,		// ºÃ–¯∞⁄ÃØ server/client; idUser: playerID; unPosX,unPosY: playerpos; unDir:dirofbooth; idTarget:idnpc;
        /** Destroy the booth */
        ACTION_DESTROY_BOOTH = 46,
        actionQueryCryOut		=47,		// ≤È—Ø∞⁄ÃØﬂ∫∫»
        actionPostCmd			=48,		// to client only
        actionQueryEquipment	=49,		// to server //idUserŒ™–Ë“™queryµƒPlayerID
        actionAbortTransform	=50,		// to server
        actionCombineSubSyn		=51,		// to client, idUser-> idSubSyn, idTarget-> idTargetSyn
        actionLanding			=52,		// to server only
        /** Signal to the client that the player picked up some money. */
        ACTION_GET_MONEY = 53, // to client only // ºÒµΩ500“‘º∞500“‘…œ«Æ£¨÷ª¥´∏¯◊‘º∫£¨dwDataŒ™ºÒµΩµƒ«Æ
        actionDischarge			=54,		// to server
        actionQueryEnemyInfo	=55,		// to server
        actionMoveStop			=56,		// data=milli secs.
        /** Kick back the player at a previous position (the client answer with ACTION_SYNCHRO) */
        ACTION_KICK_BACK = 57, // to client	idUser is Player ID, unPosX unPosY is Player pos
        actionDropMagic			=58,		// to client only, data is magic type
        actionDropSkill			=59,		// to client only, data is weapon skill type
        actionSoundEffect		=60,		// to client only, client play sound effect, dwData is monster type

        actionQueryMedal		=61,		// to server	idUser is Player ID, dwData is medal
        actionDelMedal			=62,		// to server	idUser is Player ID, dwData is medal
        ACTION_ADD_MEDAL = 63, // to client	idUser is Player ID, dwData is medal
        ACTION_SELECT_MEDAL = 64, // to server	idUser is Player ID, dwData is medal
        // Display Med panel

        actionQueryHonorTitle	=65,		// to server	idUser is Player ID, dwData is title
        actionDelHonorTitle		=66,		// to server	idUser is Player ID, dwData is title
        actionAddHonorTitle		=67,		// to client	idUser is Player ID, dwData is title
        actionSelectHonorTitle	=68,		// to server	idUser is Player ID, dwData is title
        // TODO 1 - 8
        ACTION_OPEN_DIALOG = 69, // to client only, open a dialog, dwData is id of dialog
        actionFlashStatus		=70,		// broadcast to client only, team member only. dwData is dwStatus

        actionQuerySynInfo		=71,		// ≤È—Ø∞Ô≈…–≈œ¢		// to server synicate id
        actionStudentApply		=72,		//  ¶∏∏…Í«Î’– ’ÕΩµ‹	// to server/client idUserŒ™ ¶∏∏ID dwDataŒ™ÕΩµ‹ID
        actionTeacherApply		=73,		// ÕΩµ‹…Í«Î∞› ¶		// to server/client idUserŒ™ÕΩµ‹ID dwDataŒ™ ¶∏∏ID

        actionAgreeStudentApply	=74,		// Õ¨“‚∞› ¶			// to server idUserŒ™ ¶∏∏ID dwDataŒ™ÕΩµ‹ID
        actionAgreeTeacherApply	=75,		// Õ¨“‚’– ’—ß…˙		// to server idUserŒ™ÕΩµ‹ID dwDataŒ™ ¶∏∏ID
        actionDismissStudent    =76,		// ø™≥˝—ß…˙         // to server idUserŒ™ ¶∏∏ID dwDataŒ™ÕΩµ‹ID
        actionLeaveTeacher		=77,		// ±≥≈— ¶√≈			// to server idUserŒ™ÕΩµ‹ID dwDataŒ™ ¶∏∏ID
        actionQuerySchoolMember	=78,		// ≤È—Ø ¶ÕΩ¡–±Ì		//to server //
        actionTeacherRequest	=79,		// ‘⁄–¬ ÷π§ª·÷–∑¢∞› ¶…Í«Î	// to server


        actionQueryPlayerTaskAcceptedList		=80,		// ≤È—ØÕÊº“–¸…Õµƒ“—Ω”»ŒŒÒ¡–±Ì // to server // dwDataŒ™…œ“ª¥Œ≤È—Øµƒ◊Ó∫Û“ª∏ˆID
        actionQueryPlayerTaskUnacceptedList		=81,		// ≤È—ØÕÊº“–¸…ÕµƒŒ¥Ω”»ŒŒÒ¡–±Ì // to server // dwDataŒ™…œ“ª¥Œ≤È—Øµƒ◊Ó∫Û“ª∏ˆID
        actionQueryPlayerTaskCompletedList		=82,		// ≤È—ØÕÊº“–¸…Õµƒ“—ÕÍ≥…»ŒŒÒ¡–±Ì // to server // dwDataŒ™…œ“ª¥Œ≤È—Øµƒ◊Ó∫Û“ª∏ˆID
        actionQueryPlayerMyTaskList				=83,		// ≤È—ØÕÊº“–¸…ÕµƒŒ“µƒ»ŒŒÒ¡–±Ì // to server // dwDataŒ™…œ“ª¥Œ≤È—Øµƒ◊Ó∫Û“ª∏ˆID
        actionQueryPlayerTaskDetail				=84,		// ≤È—ØÕÊº“–¸…Õ»ŒŒÒœÍœ∏ƒ⁄»› // to server // dwDataŒ™≤È—Ø»ŒŒÒID

        actionAcceptPlayerTask					=88,		// Ω” ‹»ŒŒÒ // to server // dwDataŒ™»ŒŒÒID
        actionCompletePlayerTask				=89,		  // ÕÍ≥…»ŒŒÒ // to server // dwDataŒ™»ŒŒÒID
        actionCancelPlayerTask					=90,		// ≥∑œ˙»ŒŒÒ // to server // dwDataŒ™»ŒŒÒID

        actionLockUser							=91,		// À¯∂®øÕªß∂À≤¢Õ¨≤Ω◊¯±Í∑ΩœÚ
        actionUnLockUser						=92,		// Ω‚À¯øÕªß∂À≤¢Õ¨≤Ω◊¯±Í∑ΩœÚ
        actionMagicTrack						=93,		// Õ¨≤Ω◊¯±Í∑ΩœÚ

        actionQueryStudentsOfStudent			=94,		// ≤È—ØÕΩÀÔ¡–±Ì£¨idTarget = –Ë“™≤È—ØÕΩÀÔ¡–±ÌµƒÕΩµ‹ID

        actionBurstXp							=95,		// XP±¨∑¢£¨‘ –Ì π”√XPººƒ‹
        actionChangeToRandomPos					=96,       // ÀÊª˙“∆∂Ø
        actionDisappear							=97
    };

    /** List of all PK modes. */
    enum PkMode
    {
        /** Can attack any player and monster. */
        PKMODE_FREE = 0,
        /** Can only attack monsters. */
        PKMODE_SAFE = 1,
        /** Can attack any monster and enemies. */
        PKMODE_TEAM = 2,
        /** Can attack any monster and PKers. */
        PKMODE_ARRESTMENT = 3
    };

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** The timestamp of the creation of the packet */
        int32_t Timestamp;
        /** The unique Id of the entity */
        uint32_t UniqId;
        /** The X coord of the entity */
        uint16_t PosX;
        /** The Y coord of the entity */
        uint16_t PosY;
        /** The direction of the entity */
        int32_t Direction;
        /** The data of the action */
        int32_t Data;
        /** The action Id */
        uint32_t Action;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgAction packet for the specified entity.
     *
     * @param[in]   aEntity     the entity doing the action
     * @param[in]   aData       the data of the action
     * @param[in]   aAction     the action Id
     */
    MsgAction(const Entity* aEntity, int32_t aData, Action aAction);

    /**
     * Create a message object from the specified buffer.
     * The buffer will be took by the object and the memory
     * freed when the object will be destroyed.
     *
     * If the server is on a Be architecture, all the integers
     * are swapped.
     *
     * @param[in,out] aBuf        a pointer to the buffer to take
     *                            the pointer will be set to null
     * @param[in]     aLen        the length in bytes of the buffer
     */
    MsgAction(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgAction();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

private:
    /* internal filling of the packet */
    void create(const Entity* aEntity, int32_t aData, Action aAction);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_ACTION_H_
