/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_FRIENDINFO_H_
#define _COPS_V7_EMULATOR_MSG_FRIENDINFO_H_

#include "common.h"
#include "msg.h"

class Player;

/**
 * Msg sent to the client to specify the information of a friend or enemy.
 */
class MsgFriendInfo : public Msg
{
public:
    /** Syndicate ID mask on a 32-bit integer. */
    static const int32_t MASK_SYNID = INT32_C(0x00FFFFFF);
    /** Syndicate rank shift on a 32-bit integer. */
    static const int32_t MASK_RANK_SHIFT = INT32_C(24);

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** Unique ID of the friend / enemy. */
        uint32_t UniqId;
        /** Look of the friend / enemy. */
        uint32_t Look;
        /** Level of the friend / enemy. */
        uint8_t Level;
        /** Profession of the friend / enemy. */
        uint8_t Profession;
        /** PkPoints of the friend / enemy. */
        int16_t PkPoints;
        /** Syndicate ID and rank of the friend / enemy. */
        uint32_t SynId_Rank;
        /** Mate of the friend / enemy. */
        char Mate[MAX_NAMESIZE];
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgFriendInfo packet for the friend / enemy.
     *
     * @param[in]   aPlayer    the friend / enemy
     */
   MsgFriendInfo(const Player& aPlayer);

    /* destructor */
    virtual ~MsgFriendInfo();

private:
    /* internal filling of the packet */
    void create(const Player& aPlayer);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_FRIENDINFO_H_
