/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_MAGICINFO_H_
#define _COPS_V7_EMULATOR_MSG_MAGICINFO_H_

#include "common.h"
#include "msg.h"

class Magic;

/**
 * Msg sent to the client to add a new magic skill.
 */
class MsgMagicInfo : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** Exp. of the magic skill. */
        uint32_t Exp;
        /** Id of the magic skill. */
        uint16_t Type;
        /** Level of the magic skill. */
        uint16_t Level;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgMagicInfo packet for the specified skill.
     *
     * @param[in]   aMagic   the magic skill
     */
   MsgMagicInfo(const Magic& aMagic);

    /* destructor */
    virtual ~MsgMagicInfo();

private:
    /* internal filling of the packet */
    void create(const Magic& aMagic);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_MAGICINFO_H_
