/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgregister.h"
#include "client.h"
#include "database.h"
#include "msgtalk.h"

MsgRegister :: MsgRegister(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgRegister :: ~MsgRegister()
{

}

void
MsgRegister :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);

    static const Database& db = Database::getInstance(); // singleton

    Client& client = *aClient;

    if (isValidString(mInfo->Account))
    {
        if (isValidString(mInfo->Password))
        {
            if (IS_SUCCESS(db.authenticate(client, mInfo->Account, mInfo->Password)) &&
                client.getAccountID() == mInfo->AccountUID)
            {
                if (isValidNameString(mInfo->Name))
                {
                    if (IS_SUCCESS(db.createPlayer(client, mInfo->Name, mInfo->Look, mInfo->Profession)))
                    {
                        LOG(INFO, "Created the character %s for the account %s.",
                            mInfo->Name, mInfo->Account);

                        MsgTalk msg(STR_SYSTEM_NAME, STR_ALLUSERS_NAME, STR_REPLY_OK,
                                    MsgTalk::CHANNEL_REGISTER);
                        client.send(&msg);
                    }
                    else
                    {
                        MsgTalk msg(STR_SYSTEM_NAME, STR_ALLUSERS_NAME, STR_ERROR_DUPLICATE_NAME,
                                    MsgTalk::CHANNEL_REGISTER);
                        client.send(&msg);
                    }
                }
                else
                {
                    MsgTalk msg(STR_SYSTEM_NAME, STR_ALLUSERS_NAME, STR_ERROR_ILLEGAL_NAME,
                                MsgTalk::CHANNEL_REGISTER);
                    client.send(&msg);
                }
            }
            else
            {
                client.disconnect();
            }
        }
        else
        {
            MsgTalk msg(STR_SYSTEM_NAME, STR_ALLUSERS_NAME, STR_ERROR_ILLEGAL_PASSWORD,
                        MsgTalk::CHANNEL_REGISTER);
            client.send(&msg);
        }
    }
    else
    {
        MsgTalk msg(STR_SYSTEM_NAME, STR_ALLUSERS_NAME, STR_ERROR_ILLEGAL_ACCOUNT,
                    MsgTalk::CHANNEL_REGISTER);
        client.send(&msg);
    }
}

void
MsgRegister :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->AccountUID = bswap32(info->AccountUID);
    info->Look = bswap16(info->Look);
    info->Profession = bswap16(info->Profession);
}
