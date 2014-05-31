/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_WEAPONSKILL_H_
#define _COPS_V7_EMULATOR_MSG_WEAPONSKILL_H_

#include "common.h"
#include "msg.h"

/**
 * Msg sent to the client to add a new weapon skill.
 */
class MsgWeaponSkill : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** Id of the weapon skill. */
        uint32_t Type;
        /** Level of the weapon skill. */
        uint32_t Level;
        /** Exp. of the weapon skill. */
        uint32_t Exp;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgWeaponSkill packet for the specified skill.
     *
     * @param[in]   aType    type of the skill
     * @param[in]   aLevel   level of the skill
     * @param[in]   aExp     exp of the skill
     */
   MsgWeaponSkill(uint16_t aType, uint8_t aLevel, uint32_t aExp);

    /* destructor */
    virtual ~MsgWeaponSkill();

private:
    /* internal filling of the packet */
    void create(uint16_t aType, uint8_t aLevel, uint32_t aExp);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_WEAPONSKILL_H_
