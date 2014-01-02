/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_DATABASE_H_
#define _COPS_V7_EMULATOR_DATABASE_H_

#include "common.h"
#include "env.h"
#include <QtSql/QSqlDatabase>

class QSqlQuery;
class Client;
class Player;

/**
 * The global database class for interation with the SQL-like database.
 * It is a singleton and will be created when getting the instance.
 */
class Database : public Environment::Global
{
public:
    /**
     * Get the Database singleton. If the object does not exist yet,
     * it will be created.
     *
     * @returns A reference to the singleton
     */
    static Database& getInstance();

public:
    /* destructor */
    virtual ~Database();

    /**
     * Establish a connection to the host using the specified account.
     *
     * @param[in]   aHost        the IP address or the DNS of the host
     * @param[in]   aDbName      the database name
     * @param[in]   aUserName    the account name
     * @param[in]   aPassword    the password of the account
     *
     * @retval TRUE on success
     * @returns FALSE otherwise
     */
    bool connect(const char* aHost, const char* aDbName,
                 const char* aUserName, const char* aPassword);

    /**
     * Authenticate the account/password pair sent by a client.
     *
     * @param[in]   aClient     the client
     * @param[in]   aAccount    the account name
     * @param[in]   aPassword   the password of the account
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the SQL cmd failed
     * @retval ERROR_NOT_FOUND if the account/password pair was not found
     * @returns Error code otherwise
     */
    err_t authenticate(Client& aClient, const char* aAccount, const char* aPassword);

    /**
     * Create a new player with the specified name and profession.
     *
     * @param[in]   aClient        the client
     * @param[in]   aName          the player name
     * @param[in]   aLook          the player look
     * @param[in]   aProfession    the player profession
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the SQL cmd failed
     * @returns Error code otherwise
     */
    err_t createPlayer(Client& aClient, const char* aName,
                       uint16_t aLook, uint16_t aProfession);

    /**
     * Try to retreive the player information for the specified client.
     * The player will be null if none exist.
     *
     * @param[in]   aClient     the client
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the SQL cmd failed
     * @returns Error code otherwise
     */
    err_t getPlayerInfo(Client& aClient);

    /**
     * Try to save the player information for the specified client.
     *
     * @param[in]   aClient     the client
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the SQL cmd failed
     * @returns Error code otherwise
     */
    err_t savePlayer(Client& aClient);

    /**
     * Load all maps in memory from the database.
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the SQL cmd failed
     * @returns Error code otherwise
     */
    err_t loadAllMaps();

    /**
     * Get the passway information based on the map Id and the passway Id.
     *
     * @param[out]   aOutMapId     the new map Id
     * @param[out]   aOutPosX      the new X position
     * @param[out]   aOutPosY      the new Y position
     * @param[in]    aMapId        the passway's map Id
     * @param[in]    aIndex        the passway's index
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the SQL cmd failed
     * @returns Error code otherwise
     */
    err_t getPasswayInfo(uint32_t& aOutMapId, uint16_t& aOutPosX, uint16_t& aOutPosY,
                         uint32_t aMapId, uint8_t aIndex) const;

    /**
     * Load all items in memory from the database.
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the SQL cmd failed
     * @returns Error code otherwise
     */
    err_t loadAllItems();

    /**
     * Load all NPCs in memory from the database.
     *
     * @retval ERROR_SUCCESS on success
     * @retval ERROR_EXEC_FAILED if the SQL cmd failed
     * @returns Error code otherwise
     */
    err_t loadAllNPCs();

private:
    /* constructor */
    Database();

    /** Get the SQL command from the SQL query. */
    QString getSqlCommand(const QSqlQuery& aQuery) const;

private:
    static Database* sInstance; //!< static instance of the singleton

private:
    QSqlDatabase mConnection; //!< SQL connection for the queries
};

#endif // _COPS_V7_EMULATOR_DATABASE_H_
