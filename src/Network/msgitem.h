/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_ITEM_H_
#define _COPS_V7_EMULATOR_MSG_ITEM_H_

#include "common.h"
#include "msg.h"

/**
 * Msg sent to the MsgServer to signal small item actions like equiping,
 * buying, selling, dropping...
 */
class MsgItem : public Msg
{
public:
    enum Action
    {
        /** No action specified. (Invalid) */
        ACTION_NONE = 0,
        /** Buy an item from a Npc shop. */
        ACTION_BUY = 1,             // to server, id: idNpc, data: idItemType
        /** Sell an item to a Npc shop. */
        ACTION_SELL = 2,            // to server, id: idNpc, data: idItem
        /** Drop an item on the ground. */
        ACTION_DROP = 3,            // to server, x, y
        /** Use an item (might equip). */
        ACTION_USE = 4,             // to server, data: position
        /** Equip an item at a specific position. */
        ACTION_EQUIP = 5,           // to client£¬Í¨Öª×°±¸ÎïÆ·
        /** Unequip an item at a specific position. */
        ACTION_UNEQUIP = 6,         // to server, data: position
        /** Split an item. */
        ACTION_SPLITITEM = 7,       // to server, data: num
        /** Combine an item. (Arrows ?) */
        ACTION_COMBINEITEM = 8,     // to server, data: id
        /** Query the saved money. */
        ACTION_QUERYMONEYSAVED = 9,	// to server/client, id: idNpc, data: nMoney
        /** Deposit money. */
        ACTION_SAVEMONEY = 10,      // to server, id: idNpc, data: nMoney
        /** Withdraw money. */
        ACTION_DRAWMONEY = 11,      // to server, id: idNpc, data: nMoney
        /** Drop money on the ground. */
        ACTION_DROPMONEY = 12,      // to server, x, y
        /** Spend money ??? */
        ACTION_SPENDMONEY = 13,     // ÎÞÒâÒå£¬ÔÝ±£Áô
        /** Repair an item at a Npc shop. */
        ACTION_REPAIR = 14,         // to server, return MsgItemInfo
        /** Repair all items. */
        ACTION_REPAIRALL = 15,      // to server, return ACTION_REPAIRAll, or not return if no money
        /** Ident an item. */
        ACTION_IDENT = 16,          // to server, return MsgItemInfo
        /** Update the durability of an item. */
        ACTION_DURABILITY = 17,     // to client, update durability
        /** Delete an equipment. */
        ACTION_DROPEQUIPMENT = 18,  // to client, delete equipment
        /** Improve the quality of an item. */
        ACTION_IMPROVE = 19,        // to server, improve equipment
        /** Upgrade the level of an item. */
        ACTION_UPLEV = 20,          // to server, upleve equipment
        /** Open a booth and request all items. */
        ACTION_BOOTH_QUERY = 21,    // to server, open booth, data is npc id
        /** Add an item to the booth. */
        ACTION_BOOTH_ADD = 22,      // to server/client(broadcast in table), id is idItem, data is money
        /** Delete an item from the booth. */
        ACTION_BOOTH_DEL = 23,      // to server/client(broadcast), id is idItem, data is npc id
        /** Buy an item from a booth. */
        ACTION_BOOTH_BUY = 24,      // to server, id is idItem, data is npc id
        /** Synchronise the amount (durability) of an item. */
        ACTION_SYNCHRO_AMOUNT = 25, // to client(data is amount)
        /** Use fireworks ??? */
        ACTION_FIREWORKS = 26,
        /** Ping evaluation */
        ACTION_COMPLETE_TASK = 27
    };

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;
        /** The unique Id of the item */
        uint32_t UniqId;

        union
        {
            /** The data of the action. */
            uint32_t Data;
            struct
            {
                /** The Y coord of the item. */
                uint16_t PosY;
                /** The X coord of the item. */
                uint16_t PosX;
            };
        };

        /** The action Id */
        uint32_t Action;
        /** The timestamp of the msg. */
        uint32_t Timestamp;
        /** The target unique Id. */
        uint32_t TargetUID;
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgItem packet for the specified UID.
     *
     * @param[in]   aUID       the unique ID
     * @param[in]   aAction    the action Id
     */
    MsgItem(uint32_t aUID, Action aAction);

    /**
     * Create a new MsgItem packet for the specified UID.
     *
     * @param[in]   aUID       the unique ID
     * @param[in]   aData      the data of the action
     * @param[in]   aAction    the action Id
     */
    MsgItem(uint32_t aUID, uint32_t aData, Action aAction);

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
    MsgItem(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgItem();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

private:
    /* internal filling of the packet */
    void create(uint32_t aUID, uint32_t aData, uint32_t aTargetUID, Action aAction);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_ITEM_H_
