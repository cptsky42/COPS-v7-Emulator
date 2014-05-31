/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_FLUSHEXP_H_
#define _COPS_V7_EMULATOR_MSG_FLUSHEXP_H_

#include "common.h"
#include "msg.h"

/**
 * Msg sent to the client to update the experience of a skill.
 */
class MsgFlushExp : public Msg
{
public:
   enum Action
   {
       /** Skill is a weapon skill. */
       ACTION_WEAPONSKILL = 0,
       /** Skill is a magic skill. */
       ACTION_MAGIC = 1,
       /** Skill is a regular skill. */
       ACTION_SKILL = 2
   };

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** Exp. of the skill. */
        uint32_t Exp;
        /** Id of the skill. */
        uint16_t Type;
        /** Type of skill affected. */
        uint8_t Action;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgFlushExp packet for the specified skill.
     *
     * @param[in]   aType    type of the skill
     * @param[in]   aExp     exp of the skill
     * @param[in]   aAction  type of the skill (weapon, magic, etc)
     */
   MsgFlushExp(uint16_t aType, uint32_t aExp, Action aAction);

    /* destructor */
    virtual ~MsgFlushExp();

private:
    /* internal filling of the packet */
    void create(uint16_t aType, uint32_t aExp, Action aAction);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_FLUSHEXP_H_
