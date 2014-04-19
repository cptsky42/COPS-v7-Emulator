/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msgtalk.h"
#include "stringpacker.h"
#include "allmsg.h"
#include "client.h"
#include "player.h"
#include "world.h"
#include <string.h>

MsgTalk :: MsgTalk(const char* aSpeaker, const char* aHearer, const char* aWords,
                   Channel aChannel, uint32_t aColor)
    : Msg(sizeof(MsgInfo) +
          (aSpeaker != nullptr ? strlen(aSpeaker) : 0)  + 1 +
          (aHearer != nullptr ? strlen(aHearer) : 0)  + 1 +
          /* (aEmotion != nullptr ? strlen(aEmotion) : 0) */ + 1 +
          (aWords != nullptr ? strlen(aWords) : 0) + 1),
      mInfo((MsgInfo*)mBuf)
{
    create(aSpeaker, aHearer, "", aWords, aChannel, aColor);
}

MsgTalk :: MsgTalk(const Player& aSpeaker, const Player& aHearer, const char* aWords,
                   Channel aChannel, uint32_t aColor)
    : Msg(sizeof(MsgInfo) +
          (aSpeaker.getName() != nullptr ? strlen(aSpeaker.getName()) : 0)  + 1 +
          (aHearer.getName() != nullptr ? strlen(aHearer.getName()) : 0)  + 1 +
          /* (aEmotion != nullptr ? strlen(aEmotion) : 0) */ + 1 +
          (aWords != nullptr ? strlen(aWords) : 0) + 1),
      mInfo((MsgInfo*)mBuf)
{
    create(aSpeaker, aHearer, "", aWords, aChannel, aColor);
}

MsgTalk :: MsgTalk(uint8_t** aBuf, size_t aLen)
    : Msg(aBuf, aLen), mInfo((MsgInfo*)mBuf)
{
    ASSERT(aLen >= sizeof(MsgInfo));

    #if BYTE_ORDER == BIG_ENDIAN
    swap(mBuf);
    #endif
}


MsgTalk :: ~MsgTalk()
{

}

void
MsgTalk :: create(const char* aSpeaker, const char* aHearer, const char* aEmotion,
                  const char* aWords, Channel aChannel, uint32_t aColor)
{
    ASSERT(aSpeaker != nullptr && aSpeaker[0] != '\0');
    ASSERT(aHearer != nullptr && aHearer[0] != '\0');
    ASSERT(aEmotion != nullptr);
    ASSERT(aWords != nullptr && aWords[0] != '\0');

    if (strlen(aSpeaker) < MAX_NAMESIZE &&
        strlen(aHearer) < MAX_NAMESIZE &&
        strlen(aEmotion) < MAX_NAMESIZE &&
        strlen(aWords) < MAX_WORDSSIZE)
    {
        mInfo->Header.Length = mLen;
        mInfo->Header.Type = MSG_TALK;

        mInfo->Color = aColor;
        mInfo->Channel = (uint16_t)aChannel;
        mInfo->Style = (int16_t)STYLE_NORMAL;
        mInfo->Timestamp = timeGetTime();
        mInfo->HearerLook = 0;
        mInfo->SpeakerLook = 0;

        StringPacker packer(mInfo->Buf);
        packer.addString(aSpeaker);
        packer.addString(aHearer);
        packer.addString(aEmotion);
        packer.addString(aWords);
    }
    else
    {
        LOG(ERROR, "Invalid length: hearer=%zu, speaker=%zu, emotion=%zu, words=%zu",
            strlen(aHearer), strlen(aSpeaker), strlen(aEmotion), strlen(aWords));
    }
}

void
MsgTalk :: create(const Player& aSpeaker, const Player& aHearer, const char* aEmotion,
                  const char* aWords, Channel aChannel, uint32_t aColor)
{
    ASSERT(&aSpeaker != nullptr && &aHearer != nullptr);
    ASSERT(aSpeaker.getName() != nullptr && aSpeaker.getName()[0] != '\0');
    ASSERT(aHearer.getName() != nullptr && aHearer.getName()[0] != '\0');
    ASSERT(aEmotion != nullptr);
    ASSERT(aWords != nullptr && aWords[0] != '\0');

    if (strlen(aSpeaker.getName()) < MAX_NAMESIZE &&
        strlen(aHearer.getName()) < MAX_NAMESIZE &&
        strlen(aEmotion) < MAX_NAMESIZE &&
        strlen(aWords) < MAX_WORDSSIZE)
    {
        mInfo->Header.Length = mLen;
        mInfo->Header.Type = MSG_TALK;

        mInfo->Color = aColor;
        mInfo->Channel = (uint16_t)aChannel;
        mInfo->Style = (int16_t)STYLE_NORMAL;
        mInfo->Timestamp = timeGetTime();
        mInfo->HearerLook = aHearer.getLook();
        mInfo->SpeakerLook = aSpeaker.getLook();

        StringPacker packer(mInfo->Buf);
        packer.addString(aSpeaker.getName());
        packer.addString(aHearer.getName());
        packer.addString(aEmotion);
        packer.addString(aWords);
    }
    else
    {
        LOG(ERROR, "Invalid length: hearer=%zu, speaker=%zu, emotion=%zu, words=%zu",
            strlen(aHearer.getName()), strlen(aSpeaker.getName()), strlen(aEmotion), strlen(aWords));
    }
}

void
MsgTalk :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);

    Client& client = *aClient;
    Player& player = *client.getPlayer();

    char speaker[MAX_NAMESIZE];
    char hearer[MAX_NAMESIZE];
    char words[MAX_WORDSSIZE];

    StringPacker packer(mInfo->Buf);
    packer.getString(speaker, sizeof(speaker), 0);
    packer.getString(hearer, sizeof(hearer), 1);
    packer.getString(words, sizeof(words), 3);

    ASSERT(strlen(words) < MAX_WORDSSIZE);

    if (strncmp(player.getName(), speaker, MAX_NAMESIZE) != 0)
    {
        //if (!player.isGM())
        // TODO: Cheat not same speaker...
        return;
    }

    // TODO isTalkEnable
//    if (!player.isTalkEnable())
//    {
//        player.sendSysMsg(STR_CAN_NOT_TALK);
//        return;
//    }

    // TODO isGM / isPM
//    if (player.isGM()) // log GM msgs
//    {
//        LOG(INFO, "--TALK-- %s said '%s' to %s.",
//            speaker, words, hearer);
//    }

    // commands
    if (words[0] == '/')
    {
        char cmd[MAX_WORDSSIZE] = "NO_CMD";
        char param[MAX_WORDSSIZE] = "";
        sscanf(words, "/%s %[^\n]s", cmd, param);

        if (strncmp(cmd, "break", MAX_WORDSSIZE) == 0)
        {
            client.disconnect();
        }
        else if (strncmp(cmd, "mm", MAX_WORDSSIZE) == 0)
        {
            int mapId, x, y;
            if (sscanf(param, "%d %d %d", &mapId, &x, &y) == 3)
            {
                player.move((uint32_t)mapId, (uint16_t)x, (uint16_t)y);
            }
            else
                player.sendSysMsg("USAGE: /mm id x y");
        }
        else if (strncmp(cmd, "money", MAX_WORDSSIZE) == 0)
        {
            unsigned int money;
            if (sscanf(param, "%u", &money) == 1)
            {

            }
            else
                player.sendSysMsg("USAGE: /money amount");
        }
        #ifndef NDEBUG

        #endif // not NDEBUG
        else
        {
            player.sendSysMsg("Unknown command '%s'.", cmd);
        }

        return;
    }

    if (player.isGhost() && mInfo->Channel != MsgTalk::CHANNEL_TEAM)
    {
        mInfo->Channel = MsgTalk::CHANNEL_GHOST;
        player.broadcastRoomMsg(this, false);
        return;
    }

    static const World& world = World::getInstance();
    switch (mInfo->Channel)
    {
        case MsgTalk::CHANNEL_PRIVATE:
            {
                Player* target = nullptr;
                if (world.queryPlayer(&target, hearer))
                    target->send(this);
            }
        default:
            fprintf(stdout, "%s said %s to %s on %u\n",
                    speaker, words, hearer, mInfo->Channel);
            break;
    }
}

void
MsgTalk :: swap(uint8_t* aBuf) const
{
    ASSERT(aBuf != nullptr);

    MsgInfo* info = (MsgInfo*)aBuf;

    info->Color = bswap32(info->Color);
    info->Channel = bswap16(info->Channel);
    info->Style = bswap16(info->Style);
    info->Timestamp = bswap32(info->Timestamp);
    info->HearerLook = bswap32(info->HearerLook);
    info->SpeakerLook = bswap32(info->SpeakerLook);
}
