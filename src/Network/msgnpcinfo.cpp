/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgnpcinfo.h"
#include "npc.h"
#include "stringpacker.h"

MsgNpcInfo :: MsgNpcInfo(const Npc& aNpc)
    : Msg(sizeof(MsgInfo) +
          (aNpc.getName() != nullptr ? strlen(aNpc.getName()) + 1 : 0)),
      mInfo((MsgInfo*)mBuf)
{
    create(aNpc);
}

MsgNpcInfo :: ~MsgNpcInfo()
{

}

void
MsgNpcInfo :: create(const Npc& aNpc)
{
    ASSERT(&aNpc != nullptr);

    mInfo->Header.Length = mLen;
    mInfo->Header.Type = MSG_NPCINFO;

    mInfo->UniqId = aNpc.getUID();
    mInfo->PosX = aNpc.getPosX();
    mInfo->PosY = aNpc.getPosY();
    mInfo->Look = (int16_t)aNpc.getLook();
    mInfo->Type = aNpc.getType();
    mInfo->Sort = aNpc.getSort();
    mInfo->Length = 0; // Unused by EoF
    mInfo->Fat = 0; // Unused by EoF
    mInfo->Buf[0] = 0;

    if (aNpc.getName() != nullptr)
    {
        if (strlen(aNpc.getName()) < MAX_NAMESIZE)
        {
            StringPacker packer(mInfo->Buf);
            packer.addString(aNpc.getName());
        }
        else
        {
            LOG(ERROR, "Invalid length: name=%zu",
                strlen(aNpc.getName()));
        }
    }
}

void
MsgNpcInfo :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->UniqId = bswap32(info->UniqId);
    info->PosX = bswap16(info->PosX);
    info->PosY = bswap16(info->PosY);
    info->Look = bswap16(info->Look);
    info->Type = bswap16(info->Type);
    info->Sort = bswap16(info->Sort);
}

