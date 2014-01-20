/*
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
        ACTION_CHG_DIR = 79,
        /** Change the emotion of an entity */
        ACTION_EMOTION = 81,
        /** Signal to the server that the client entered in a portal. */
        ACTION_CHG_MAP = 85,
        /** Signal to the client that the player is changing map. */
        ACTION_FLY_MAP = 86,
        /** Set the position of an entity entering in a new map */
        ACTION_ENTER_MAP = 74,
        /** Get the items (client request). The next step is the friends. */
        ACTION_GET_ITEM_SET = 75,
        /** Get the friends (client request). The next step is the weapon skills. */
        ACTION_GET_GOOD_FRIEND = 76,
        /** Signal to the client that the entity leaved the map. */
        ACTION_LEAVE_MAP = 132,
        /** Signal to both sides that an entity jumped. */
        ACTION_JUMP = 133,
        /** Signal to the client that the player gained a level */
        ACTION_UP_LEVEL = 92,
        /** Signal to the client that the XP is done. */
        ACTION_XP_CLEAR = 93,
        /** Signal to both sides that the entity will reborn. */
        ACTION_REBORN = 94,
        /** Delete the current role. */
        ACTION_DELETE_ROLE = 95,
        /** Get the weapon skills (client request). The next step is the magics. */
        ACTION_GET_WEAPON_SKILL_SET = 77,
        /** Get the magics (client request). The next step is the legion attributes. */
        ACTION_GET_MAGIC_SET = 78,
        /** Set the Pk mode of an entity */
        ACTION_SET_PKMODE = 96,
        /** Get the legion attributes. */
        ACTION_GET_SYN_ATTR = 97,
        /** Signal to the client to transform in a ghost. */
        ACTION_GHOST = 137,
        /** Synchronize the position (the client will send its coords) as answer */
        ACTION_SYNCRHO = 138, // Unknown action[0032], data=[54788624]
        /** Change the face of the entity */
        ACTION_CHANGE_FACE = 142,
        /** Signal to both sides that the entity is mining. */
        ACTION_MINE = 99,
        /** Request a player to the server. */
        ACTION_QUERY_PLAYER = 102,
        /** Set the color of the map */
        ACTION_MAP_ARGB = 104,
        /** Signal to client/server a new booth. */
        ACTION_CREATE_BOOTH = 111, // UID: playerID; PosX,Y: playerpos; unDir:dirofbooth; idTarget:idnpc;
        /** Destroy the booth */
        ACTION_DESTROY_BOOTH = 114,
        /** Post a command to the client. */
        ACTION_POST_CMD = 116, // to client only
        /** Abort the current transformation. */
        ACTION_ABORT_TRANSFORM = 118, // to server
        /** Signal to the client that the player picked up some money. */
        ACTION_GET_MONEY = 121, // to client only // ºÒµΩ500“‘º∞500“‘…œ«Æ£¨÷ª¥´∏¯◊‘º∫£¨dwDataŒ™ºÒµΩµƒ«Æ
        /** Kick back the player at a previous position (the client answer with ACTION_SYNCHRO) */
        ACTION_KICK_BACK = 108, // to client    idUser is Player ID, unPosX unPosY is Player pos
        /** Open a dialog on the client. */
        ACTION_OPEN_DIALOG = 126 // to client only, open a dialog, dwData is id of dialog
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
        /** The data of the action */
        int32_t Data;
        /** The X coord of the entity */
        uint16_t PosX;
        /** The Y coord of the entity */
        uint16_t PosY;
        /** The direction of the entity */
        uint16_t Direction;
        /** The action Id */
        uint16_t Action;
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
