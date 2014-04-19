/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_CLIENT_H_
#define _COPS_V7_EMULATOR_CLIENT_H_

#include "common.h"
#include "icipher.h"
#include <string>

class DiffieHellman;
class Database;
class NetworkClient;
class Msg;
class Player;
class NpcTask;

/**
 * Assossiated client to a NetworkClient.
 */
class Client
{
    friend class Database; // the database can manipulate the data

    // !!! class has pointer data members !!!
    PROHIBIT_COPY(Client);

public:
    /** The status of the client. */
    enum Status
    {
        /** The client is still not authenticated */
        NOT_AUTHENTICATED,
        /** The client is authenticated, but is doing the key exchange */
        KEY_EXCHANGE,
        /** The client is banned */
        BANNED,
        /** The client use an invalid username */
        INVALID_USERNAME,
        /** The client use an invalid password */
        INVALID_PASSWORD,
        /** The client use an invalid token */
        INVALID_KEY,
        /** The client is normal */
        NORMAL,
        /** The client is a new account */
        NEW_ACCOUNT
    };

public:
    /* constructor */
    Client(NetworkClient* aSocket, ICipher::Algorithm aAlgorithm);

    /* destructor */
    ~Client();

    /**
     * Save the player.
     */
    void save();

    /**
     * Generate the exchange request and send it to the client.
     */
    void generateExchangeRequest();

    /**
     * Handle the client response to the exchange request.
     *
     * @param[in]   aBuf    the MsgLoginChallengeS buffer
     * @param[in]   aLen    the length in bytes of the buffer
     */
    void handleExchangeResponse(uint8_t** aBuf, size_t aLen);

    /**
     * Send a message to the client.
     * The TQ seal will be appended at the end.
     *
     * @param[in]  aMsg     the message to send
     */
    void send(const Msg* aMsg);

    /**
     * Disconnect the client from the server.
     */
    void disconnect();

public:
    /** Get a reference to the client cipher */
    ICipher& getCipher() const { return *mCipher; }
    /** Get a reference to the client exchange */
    DiffieHellman& getExchange() const { return *mExchange; }

    /** Get the account name of the client. */
    const char* getAccount() const { return mAccount.c_str(); }
    /** Get the account UID of the client. */
    int32_t getAccountID() const { return mAccountID; }
    /** Get the status of the account */
    Status getStatus() const { return mStatus; }

    /** Get the current Npc task. */
    const NpcTask* getCurTask() const { return mCurTask; }

    /** Get the player object linked to this client */
    Player* getPlayer() const { return mPlayer; }

    /** Set the account name of the client. */
    void setAccount(const std::string& aAccount) { mAccount = aAccount; }
    /** Set the account ID of the client. */
    void setAccountID(int32_t aAccountID) { mAccountID = aAccountID; }
    /** Set the client status. */
    void setStatus(Status aStatus) { mStatus = aStatus; }

    /** Set the current Npc task. */
    void setCurTask(const NpcTask& aTask) { mCurTask = &aTask; }

private:
    NetworkClient* mSocket; //!< the TCP/IP socket wrapper of the client
    ICipher* mCipher; //!< the cipher of the client

    DiffieHellman* mExchange; //!< the key exchange of the client
    uint8_t* mEncryptIV; //!< the new encryption IV (in the exchange)
    uint8_t* mDecryptIV;  //!< the new decryption IV (in the exchange)

    Status mStatus; //!< the status of the account

    std::string mAccount; //!< the account name
    int32_t mAccountID; //!< the account ID
    int8_t mAccLvl; //!< the account permissions level
    int32_t mFlags; //!< the account flags
    std::string mCharacter; //!< the character name

    Player* mPlayer; //!< the character linked to this client

    const NpcTask* mCurTask; //!< the current NPC task
};

#endif // _COPS_V7_EMULATOR_CLIENT_H_
