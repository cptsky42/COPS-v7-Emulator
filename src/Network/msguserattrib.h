/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_USERATTRIB_H_
#define _COPS_V7_EMULATOR_MSG_USERATTRIB_H_

#include "common.h"
#include "msg.h"

class Entity;

class MsgUserAttrib : public Msg
{
public:
    enum UserAttrType
    {
        USER_ATTRIB_LIFE = 0,
        USER_ATTRIB_MAX_LIFE = 1,
        USER_ATTRIB_MANA = 2,
        USER_ATTRIB_MAX_MANA = 3,
        USER_ATTRIB_MONEY = 4,
        USER_ATTRIB_EXP = 5,
        USER_ATTRIB_PK = 6,
        USER_ATTRIB_PROFESSION = 7,
        USER_ATTRIB_SIZE_ADD = 8, //0 -> None / 1 -> Cursed / 2 -> Blessed / 3 -> Cursed & Blessed (Important for these effect!)
        USER_ATTRIB_ENERGY = 9,
        USER_ATTRIB_MONEY_SAVED = 10,
        USER_ATTRIB_ADD_POINT = 11,
        USER_ATTRIB_LOOK_FACE = 12,
        USER_ATTRIB_LEV = 13,
        USER_ATTRIB_SOUL = 14,
        USER_ATTRIB_HEALTH = 15,
        USER_ATTRIB_FORCE = 16,
        USER_ATTRIB_DEXTERITY = 17,
        USER_ATTRIB_BLESS_TIME = 18,
        USER_ATTRIB_DBL_EXP_TIME = 19,
        // 20 = GuildDonation (when you attack the GW pole for example, this is sent with your new donation)
        USER_ATTRIB_CURSE_TIME = 21,
        USER_ATTRIB_TIME_ADD = 22, //Cyclone & Superman (0 = 0sec / 1 = 1sec / 2 = 2 sec / 3 = 3sec)
        USER_ATTRIB_METEMPSYCHOSIS = 23,
        USER_ATTRIB_STATUS = 26,
        USER_ATTRIB_HAIR = 27,
        USER_ATTRIB_XP = 28,
        USER_ATTRIB_LUCKY_TIME = 29,
        USER_ATTRIB_CPS = 30,
        USER_ATTRIB_TRAINING_POINTS = 32 //0 -> Show / 1 -> In Offline TG (No Points) / 2 -> Out Offline TG (Points) / 3 -> Add 10 pts / 4 -> 100%
    };

public:
    #pragma pack(push, 1)
    typedef struct
    {
        uint32_t Type;
        uint64_t Data;
    }UserAttrib;
    #pragma pack(pop)

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        uint32_t UniqId;
        uint32_t Amount;
        UserAttrib Attrib[1];
    }MsgInfo;
    #pragma pack(pop)

public:
    MsgUserAttrib(Entity* aEntity, uint64_t aData, UserAttrType aType);

    /* destructor */
    virtual ~MsgUserAttrib();

private:
    /* internal filling of the packet */
    void create(Entity* aEntity, uint64_t* aData, UserAttrType aType);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_USERATTRIB_H_
