/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_USERINFO_H_
#define _COPS_V7_EMULATOR_MSG_USERINFO_H_

#include "common.h"
#include "msg.h"

class Player;

/**
 * Msg sent to the client by the MsgServer to fill all the player variables.
 */
class MsgUserInfo : public Msg
{
public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** The unique Id of the player */
        uint32_t UniqId;
        /** The look of the player */
        uint32_t Look;
        /** The hair of the player */
        uint16_t Hair;
        /** The length of the player (unused) */
        int8_t Length;
        /** The fat of the player (unused) */
        int8_t Fat;
        /** The money of the player */
        int32_t Money;
        /** The experience of the player */
        uint32_t Exp;
        /** Unknown bytes (padding ?) */
        uint8_t Padding1[10];
        /** The mercenary exp of the player */
        uint16_t MercenaryExp;
        /** The mercenary level of the player */
        uint16_t MercenaryLevel;
        /** Unknown bytes (padding ?) */
        uint8_t Padding2[14];
        /** The force of the player */
        uint16_t Force;
        /** The dexterity of the player */
        uint16_t Dexterity;
        /** The health of the player */
        uint16_t Health;
        /** The soul of the player */
        uint16_t Soul;
        /** The additional points of the player */
        uint16_t AddPoints;
        /** The hit points of the player */
        uint16_t CurHP;
        /** The mana points of the player */
        uint16_t CurMP;
        /** The pk points of the player */
        int16_t PkPoints;
        /** The level of the player */
        uint8_t Level;
        /** The profession of the player */
        uint8_t Profession;
        /** If the server handle the allot points */
        uint8_t AutoAllot;
        /** The metempsychosis of the player */
        uint8_t Metempsychosis;
        /** If the client must show the name of the player */
        uint8_t ShowName;
        /** Name / Mate of the player */
        uint8_t Buf[1];
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgUserInfo packet for the specified player.
     *
     * @param[in]   aPlayer     a reference to the player object
     */
    MsgUserInfo(const Player& aPlayer);

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
    MsgUserInfo(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgUserInfo();

private:
    /* internal filling of the packet */
    void create(const Player& aPlayer);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_USERINFO_H_
