/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "msg.h"
#include "allmsg.h"
#include "client.h"

//#include <xmmintrin.h>
//static void X_aligned_memcpy_sse2(void* dest, const void* src, const unsigned long size_t)
//{
//  __asm
//  {
//    mov esi, src;    //src pointer
//    mov edi, dest;   //dest pointer

//    mov ebx, size_t; //ebx is our counter
//    shr ebx, 7;      //divide by 128 (8 * 128bit registers)


//    loop_copy:
//      prefetchnta 128[ESI]; //SSE2 prefetch
//      prefetchnta 160[ESI];
//      prefetchnta 192[ESI];
//      prefetchnta 224[ESI];

//      movdqa xmm0, 0[ESI]; //move data from src to registers
//      movdqa xmm1, 16[ESI];
//      movdqa xmm2, 32[ESI];
//      movdqa xmm3, 48[ESI];
//      movdqa xmm4, 64[ESI];
//      movdqa xmm5, 80[ESI];
//      movdqa xmm6, 96[ESI];
//      movdqa xmm7, 112[ESI];

//      movntdq 0[EDI], xmm0; //move data from registers to dest
//      movntdq 16[EDI], xmm1;
//      movntdq 32[EDI], xmm2;
//      movntdq 48[EDI], xmm3;
//      movntdq 64[EDI], xmm4;
//      movntdq 80[EDI], xmm5;
//      movntdq 96[EDI], xmm6;
//      movntdq 112[EDI], xmm7;

//      add esi, 128;
//      add edi, 128;
//      dec ebx;

//      jnz loop_copy; //loop please
//    loop_copy_end:
//  }
//}

/* static */
void
Msg :: create(Msg** aOutMsg, uint8_t** aBuf, size_t aLen)
{
    ASSERT(aOutMsg != nullptr && *aOutMsg == nullptr);
    ASSERT(aBuf != nullptr && *aBuf != nullptr);
    ASSERT(aLen >= sizeof(Msg::Header));

    Msg* msg = nullptr;
    Msg::Header* header = (Msg::Header*)(*aBuf);
    switch (header->Type)
    {
        case MSG_ACCOUNT:
            msg = new MsgAccount(aBuf, aLen);
            break;
        case MSG_CONNECT:
            msg = new MsgConnect(aBuf, aLen);
            break;
        case MSG_REGISTER:
            msg = new MsgRegister(aBuf, aLen);
            break;
        case MSG_TALK:
            msg = new MsgTalk(aBuf, aLen);
            break;
        case MSG_TICK:
            msg = new MsgTick(aBuf, aLen);
            break;
        case MSG_WALK:
            msg = new MsgWalk(aBuf, aLen);
            break;
        case MSG_USERINFO:
            msg = new MsgUserInfo(aBuf, aLen);
            break;
        case MSG_ACTION:
            msg = new MsgAction(aBuf, aLen);
            break;
        case MSG_USERATTRIB:
            msg = new MsgUserAttrib(aBuf, aLen);
            break;
        case MSG_ITEMINFO:
            msg = new MsgItemInfo(aBuf, aLen);
            break;
        case MSG_DIALOG:
            msg = new MsgDialog(aBuf, aLen);
            break;
        case MSG_NPC:
            msg = new MsgNpc(aBuf, aLen);
            break;
        case MSG_ALLOT:
            msg = new MsgAllot(aBuf, aLen);
            break;
        default:
            msg = new Msg(aBuf, aLen);
            break;
    }

    *aOutMsg = msg;
}

Msg :: Msg(uint8_t** aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr && *aBuf != nullptr);
    ASSERT(aLen >= sizeof(Msg::Header));

    mLen = aLen;
    mBuf = *aBuf;

    #if BYTE_ORDER == BIG_ENDIAN
    mData = new uint8_t[mLen];
    #endif

    *aBuf = nullptr;
}

Msg :: Msg(size_t aLen)
{
    mLen = aLen;
    mBuf = new uint8_t[mLen];
    memset(mBuf, 0, mLen);

    #if BYTE_ORDER == BIG_ENDIAN
    mData = new uint8_t[mLen];
    #endif
}

Msg :: ~Msg()
{
    SAFE_DELETE_ARRAY(mBuf);
    #if BYTE_ORDER == BIG_ENDIAN
    SAFE_DELETE_ARRAY(mData);
    #endif
}

void
Msg :: process(Client* aClient)
{
    ASSERT(aClient != nullptr);

    Msg::Header* header = (Msg::Header*)mBuf;
    fprintf(stdout, "Unknown msg[%04d], len=[%03d]\n",
            header->Type, header->Length);

    dump(this);
}

/* static */
void
Msg :: dump(Msg* aMsg)
{
    ASSERT(aMsg != nullptr);

    dump(aMsg->getBuffer(), aMsg->getLength());
}

/* static */
void
Msg :: dump(const uint8_t* aBuf, size_t aLen)
{
    ASSERT(aBuf != nullptr);

    const size_t AMOUNT_PER_LINE = 16;
    const uint8_t* buf = aBuf;

    for (size_t i = 0, len = aLen; i < len; i += AMOUNT_PER_LINE)
    {
        for (size_t j = 0; j < AMOUNT_PER_LINE; ++j)
        {
            if (i + j < len)
            {
                fprintf(stdout, "%02X ", buf[i + j]);
            }
            else
            {
                printf("   ");
            }
        }
        fprintf(stdout, " ");
        for (size_t j = 0; j < AMOUNT_PER_LINE; ++j)
        {
            if (i + j < len)
            {
                fprintf(stdout, "%c",
                        isprint(buf[i + j]) ? buf[i + j] : '.');
            }
        }
        fprintf(stdout, "\n");
    }
}

const uint8_t*
Msg :: getBuffer() const
{
    #if BYTE_ORDER == BIG_ENDIAN
    memcpy(mData, mBuf, mLen);

    Msg::Header* header = (Msg::Header*)mData;
    header->Length = bswap16(header->Length);
    header->Type = bswap16(header->Type);
    swap(mData);

    return mData;
    #else
    return mBuf;
    #endif
}
