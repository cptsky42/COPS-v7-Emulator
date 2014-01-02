/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_DIALOG_H_
#define _COPS_V7_EMULATOR_MSG_DIALOG_H_

#include "common.h"
#include "msg.h"

/**
 * Message used for NPC's dialogs.
 */
class MsgDialog : public Msg
{
public:
    enum Action
    {
        /** No action specified. (Invalid) */
        ACTION_NONE = 0,
        /** Text of the dialog. */
        ACTION_TEXT = 1,
        /** Options (with next task ID). */
        ACTION_LINK = 2,
        /** Input (with next task ID). */
        ACTION_EDIT = 3,
        /** Pic of the NPC. */
        ACTION_PIC = 4,
        ACTION_LISTLINE = 5,
        ACTION_CREATE = 100, // idxTask => default
        ACTION_ANSWER = 101, // to server
        ACTION_TASKID = 102  // to server, launch task if by interface
    };

public:
    #pragma pack(push, 1)
    typedef struct
    {
        /** Generic header of all msgs */
        Msg::Header Header;

        union
        {
            int32_t TaskId;
            struct
            {
                uint16_t PosX;
                uint16_t PosY;
            };
        };

        /** Data of the action (e.g. Pic) */
        uint16_t Data;
        /** Task's index (0 is default, 255 is end). */
        uint8_t IdxTask;
        /** Action Id. */
        uint8_t Action;
        /** Text of the action. */
        uint8_t Buf[1];
    }MsgInfo;
    #pragma pack(pop)

public:
    /**
     * Create a new MsgDialog packet with the specified text and data.
     *
     * @param[in]   aText       the action's text
     * @param[in]   aData       the action's data
     * @param[in]   aIdxTask    the task's index
     * @param[in]   aAction     the action Id
     */
    MsgDialog(const char* aText, uint16_t aData, uint8_t aIdxTask, Action aAction);

    /**
     * Create a new MsgDialog packet with the specified coords and data.
     *
     * @param[in]   aPosX       the X coord
     * @param[in]   aPosY       the Y coord
     * @param[in]   aPic        the pic Id
     * @param[in]   aIdxTask    the task's index
     * @param[in]   aAction     the action Id
     */
    MsgDialog(uint16_t aPosX, uint16_t aPosY, uint16_t aPic, uint8_t aIdxTask, Action aAction);

    /**
     * Create a new MsgDialog packet with empty text and data.
     *
     * @param[in]   aIdxTask    the task's index
     * @param[in]   aAction     the action Id
     */
    MsgDialog(uint8_t aIdxTask, Action aAction);

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
    MsgDialog(uint8_t** aBuf, size_t aLen);

    /* destructor */
    virtual ~MsgDialog();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

private:
    /* internal filling of the packet */
    void create(const char* aText, uint16_t aData, uint8_t aIdxTask, Action aAction);
    void create(uint16_t aPosX, uint16_t aPosY, uint16_t aPic, uint8_t aIdxTask, Action aAction);
    void create(uint8_t aIdxTask, Action aAction);

    /* internal swapping of the integers for neutral-endian support */
    virtual void swap(uint8_t* aBuf) const;

private:
    MsgInfo* mInfo; //!< the casted internal reference to the buffer
};

#endif // _COPS_V7_EMULATOR_MSG_DIALOG_H_
