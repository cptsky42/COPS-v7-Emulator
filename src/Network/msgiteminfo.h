/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_ITEMINFO_H_
#define _COPS_V7_EMULATOR_MSG_ITEMINFO_H_

#include "common.h"
#include "msg.h"

/**
 * Message describing all caracteristics of an item.
 */
class MsgItemInfo : public Msg
{
public:
    enum Action
    {
        /** No action (invalid) */
        ACTION_NONE = 0,
        /** Add an item to a specific position. */
        ACTION_ADD_ITEM = 1,
        /** Trade an item. */
        ACTION_TRADE = 2,
        /** Update the caracteristics of an item. */
        ACTION_UPDATE = 3,
        /** Describe an item equiped by another player. */
        ACTION_OTHER_PLAYER_EQUIP = 4 // uid is user uid
    };

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** Unique Id of the item. */
        uint32_t UniqId;
        /** Type of the item. */
        uint32_t Type;
        /** Actual amount of the item. */
        uint16_t Amount;
        /** Maximum amount of the item. */
        uint16_t AmountLimit;
        /** Action Id. */
        uint8_t Action;
        /** Unknown ? What is ident for an item ? */
        uint8_t Ident;
        /** Position of the item. */
        uint8_t Position;
        /** Unknown padding. */
        uint8_t Padding[5];
        /** First socket of the item. */
        uint8_t Gem1;
        /** Second socket of the item. */
        uint8_t Gem2;
        /** Attribute of the item. (e.g. poison) */
        uint8_t Magic1; // Attr
        /** Unknown attribute. */
        uint8_t Magic2; // ???
        /** Craft of the item. */
        uint8_t Magic3; // Plus
        /** Bless of the item. */
        uint8_t Bless;
        /** Enchant of the item. */
        uint8_t Enchant;
        /** Restrain of the item. */
        uint32_t Restrain;
        /** Whether the item is locked or not. */
        uint8_t Locked; // boolean
        /** Whether the item is suspicious or not. */
        uint8_t Suspicious; // boolean
        /** Color of the item. */
        uint32_t Color;
    }MsgInfo;
    #pragma pack(pop)

public:
    MsgItemInfo(void* aItem, Action aAction); // FIXME!

    /* destructor */
    virtual ~MsgItemInfo();

private:
    /* internal filling of the packet */
    void create(void* aItem, Action aAction); // FIXME!

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_ITEMINFO_H_
