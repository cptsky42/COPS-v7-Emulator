/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_PLAYER_H_
#define _COPS_V7_EMULATOR_MSG_PLAYER_H_

#include "common.h"
#include "msg.h"

class Player;
class Monster;

/**
 * Msg sent by the MsgServer to spawn a player or a monster.
 */
class MsgPlayer : public Msg
{
public:
    /** Legion ID mask on a 32-bit integer. */
    static const int32_t MASK_SYNID = INT32_C(0x00FFFFFF);
    /** Legion rank shift on a 32-bit integer. */
    static const int32_t MASK_RANK_SHIFT = INT32_C(24);

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** The unique Id of the entity */
        uint32_t UniqId;
        /** The look of the entity */
        uint32_t Look;

        union
        {
            /** The status of the entity */
            uint64_t Status;
            struct
            {
                /** The life of the statuary */
                uint16_t StatuaryLife;
                /** The frame of the statuary */
                uint16_t StaturayFrame;
            };
        };

        union
        {
            /** Legion Id and rank of the player */
            int32_t SynId_Rank;
            /** Owner unique Id of the monster. */
            int32_t OwnerUID;
        };

        /** Garment Id. */
        int32_t GarmentType;
        /** Armet Id. */
        int32_t ArmetType;
        /** Armor Id. */
        int32_t ArmorType;
        /** Right hand weapon Id. */
        int32_t WeaponRType;
        /** Left hand weapon Id. */
        int32_t WeaponLType;

        /** Unknown padding. */
        int8_t Padding1[4];

        /** Current life of the monster. */
        uint16_t Life;
        /** Level of the monster. */
        uint16_t Level;

        /** The X coord of the entity. */
        uint16_t PosX;
        /** The Y coord of the entity. */
        uint16_t PosY;
        /** The hair of the player. */
        uint16_t Hair;
        /** The cardinal direction of the entity. */
        uint8_t Direction;
        /**  The pose of the entity. */
        uint8_t Pose;
        /** The metempsychosis of the player. */
        uint16_t Metempsychosis;
        /** The level of the player */
        uint16_t PlayerLevel;

        /** Unknown padding. */
        int8_t Padding2[4];

        /** Nobility rank of the entity. */
        uint32_t NobilityRank;
        /** Nobility UID of the entity (UID). */
        uint32_t NobilityUID; // entity UID
        /** Nobility position of the enity. */
        uint32_t NobilityPos;

        /** Unknown padding. */
        int8_t Padding3[4];

        uint16_t ArmetColor;
        uint16_t ArmorColor;

        /** Unknown padding. */
        uint8_t Padding4[2];

        /** The name of the entity */
        uint8_t Buf[1];
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgPlayer packet for the specified player.
     *
     * @param[in]   aPlayer     a reference to the player object
     */
    MsgPlayer(const Player& aPlayer);

    /**
     * Create a new MsgPlayer packet for the specified monster.
     *
     * @param[in]   aMonster     a reference to the monster object
     */
    MsgPlayer(const Monster& aMonster); //TODO...

    /* destructor */
    virtual ~MsgPlayer();

private:
    /* internal filling of the packet */
    void create(const Player& aPlayer);
    void create(const Monster& aMonster);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_PLAYER_H_
