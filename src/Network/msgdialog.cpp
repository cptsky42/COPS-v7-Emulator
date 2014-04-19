/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgdialog.h"
#include "stringpacker.h"
#include "client.h"
#include "player.h"
#include "npctask.h"
#include <string.h>

MsgDialog :: MsgDialog(const char* aText, uint16_t aData,
                       uint8_t aIdxTask, Action aAction)
    : Msg(sizeof(MsgInfo) +
          strlen(aText) + 1), mInfo((MsgInfo*)mBuf)
{
    create(aText, aData, aIdxTask, aAction);
}

MsgDialog :: MsgDialog(uint16_t aPosX, uint16_t aPosY, uint16_t aPic,
                       uint8_t aIdxTask, Action aAction)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aPosX, aPosY, aPic, aIdxTask, aAction);
}

MsgDialog :: MsgDialog(uint8_t aIdxTask, Action aAction)
    : Msg(sizeof(MsgInfo)), mInfo((MsgInfo*)mBuf)
{
    create(aIdxTask, aAction);
}

MsgDialog :: MsgDialog(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

#if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
#endif
}

MsgDialog :: ~MsgDialog()
{

}

void
MsgDialog :: create(const char* aText, uint16_t aData,
                    uint8_t aIdxTask, Action aAction)
{
    ASSERT(aText != nullptr && aText[0] != '\0');

    if (strlen(aText) < MAX_PARAMSIZE)
    {
        mInfo->Header.Length = mLen;
        mInfo->Header.Type = MSG_DIALOG;

        mInfo->PosX = 0;
        mInfo->PosY = 0;
        mInfo->Data = aData;
        mInfo->IdxTask = aIdxTask;
        mInfo->Action = (uint8_t)aAction;

        StringPacker packer(mInfo->Buf);
        packer.addString(aText);
    }
    else
    {
        LOG(ERROR, "Invalid length: text=%zu",
            strlen(aText));
    }
}

void
MsgDialog :: create(uint16_t aPosX, uint16_t aPosY, uint16_t aPic,
                    uint8_t aIdxTask, Action aAction)
{
    ASSERT(aAction == ACTION_PIC);

    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_DIALOG;

    mInfo->PosX = aPosX;
    mInfo->PosY = aPosY;
    mInfo->Data = aPic;
    mInfo->IdxTask = aIdxTask;
    mInfo->Action = (uint8_t)aAction;
    mInfo->Buf[0] = 0;
}

void
MsgDialog :: create(uint8_t aIdxTask, Action aAction)
{
    ASSERT(aAction == ACTION_CREATE);

    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_DIALOG;

    mInfo->PosX = 0;
    mInfo->PosY = 0;
    mInfo->Data = 0;
    mInfo->IdxTask = aIdxTask;
    mInfo->Action = (uint8_t)aAction;
    mInfo->Buf[0] = 0;
}

void
MsgDialog :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);
    ASSERT(aClient->getPlayer() != nullptr);

    Client& client = *aClient;
    Player& player = *aClient->getPlayer();

    if (!player.isAlive())
    {
        player.sendSysMsg(STR_DIE);
        return;
    }

    char text[MAX_PARAMSIZE];

    StringPacker packer(mInfo->Buf);
    packer.getString(text, sizeof(text), 0);

    fprintf(stderr, "MsgDialog(action=%u, idx=%u, data=%u, str='%s')\n",
            mInfo->Action, mInfo->IdxTask, mInfo->Data, text);

    switch (mInfo->Action)
    {
        case ACTION_ANSWER:
            {
                // TODO /player.processTask(info->IdxTask, text);
                const NpcTask* task = client.getCurTask();

                if (task != nullptr)
                {
                    LOG(DBG, "Executing task %u for idx=%u...",
                        task->getUID(), mInfo->IdxTask);
                    task->execute(client, mInfo->IdxTask);
                }

                break;
            }
        case ACTION_TASKID:
            {
                //player.processClientTask(info->TaskId, text);
                break;
            }
        default:
            {
                fprintf(stdout, "Unknown event[%04u], data=[%d]\n",
                        mInfo->Action, mInfo->IdxTask);
                break;
            }
            //    default:
            //        ASSERT(false);
            //        break;
    }
}

void
MsgDialog :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    if (info->Action == ACTION_TASKID)
    {
        info->TaskId = bswap32(info->TaskId);
    }
    else
    {
        info->PosX = bswap16(info->PosX);
        info->PosY = bswap16(info->PosY);
    }
    info->Data = bswap16(info->Data);
}
