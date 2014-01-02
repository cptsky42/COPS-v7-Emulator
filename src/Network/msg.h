/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_MSG_H_
#define _COPS_V7_EMULATOR_MSG_H_

#include "log.h"
#include "common.h"
#include "networkdef.h"

class Client;

/**
 * The base class for all networking messages of the server.
 */
class Msg
{
    // !!! class has pointer data members !!!
    PROHIBIT_COPY(Msg);

public:
    /**
     * The general header prefixing all messages.
     */
    typedef struct
    {
        /** The length of the message in bytes */
        uint16_t Length;
        /** The type of the message */
        uint16_t Type;
    }Header;

public:
    /**
     * Create a message object from the specified buffer.
     * The buffer will be took by the object and the memory
     * freed when the object will be destroyed.
     *
     * @param[out]    aOutMsg     the msg object
     * @param[in,out] aBuf        a pointer to the buffer to take
     *                            the pointer will be set to null
     * @param[in]     aLen        the length in bytes of the buffer
     */
    static void create(Msg** aOutMsg, uint8_t** aBuf, size_t aLen);

    /**
     * Print the msg in the standard output stream.
     */
    static void dump(const uint8_t* aBuf, size_t aLen);

    /**
     * Print the msg in the standard output stream.
     */
    static void dump(Msg* aMsg);

public:
    /**
     * Delete the message and the buffer.
     */
    virtual ~Msg();

    /**
     * Process the message received from the client.
     *
     * @param[in]     aClient      a pointer to the client which
     *                             has sent the message
     */
    virtual void process(Client* aClient);

public:
    /** Get a pointer of the buffer. It may not be the internal one. */
    const uint8_t* getBuffer() const;

    /** Get the length in bytes of the message. */
    size_t getLength() const { return mLen; }

protected:
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
    Msg(uint8_t** aBuf, size_t aLen);

    /**
     * Create a message object with an internal buffer of
     * the specified length.
     *
     * @param[in]     aLen        the length in bytes of the buffer
     */
    Msg(size_t aLen);

    /**
     * Swap all integers to be valid on a Be architecture.
     *
     * @param[in]     aBuf        a pointer to the buffer to swap
     */
    virtual void swap(uint8_t* aBuf) const { aBuf = nullptr; }

protected:
    uint8_t* mBuf; //!< the internal buffer
    size_t mLen; //!< the length in bytes of the buffer

    #if BYTE_ORDER == BIG_ENDIAN
    mutable uint8_t* mData; //!< the swapped copy of the buffer
    #endif
};

#endif // _COPS_V7_EMULATOR_MSG_H_
