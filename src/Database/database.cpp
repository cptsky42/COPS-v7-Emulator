/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "log.h"
#include "database.h"
#include "client.h"
#include "world.h"
#include "mapmanager.h"
#include "player.h"
#include "npc.h"
#include "item.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QVariant>
#include <QSqlError>

/* static */
Database* Database::sInstance = nullptr;

/* static */
Database&
Database :: getInstance()
{
    // TODO? Thread-safe
    if (sInstance == nullptr)
    {
        sInstance = new Database();
    }
    return *sInstance;
}

Database :: Database()
{
    mConnection = QSqlDatabase::addDatabase("QMYSQL");
}

Database :: ~Database()
{
    if (mConnection.isOpen())
    {
        mConnection.close();
    }
}

bool
Database :: connect(const char* aHost, const char* aDbName,
                    const char* aUserName, const char* aPassword)
{
    ASSERT(aHost != nullptr && aHost[0] != '\0');
    ASSERT(aDbName != nullptr && aDbName[0] != '\0');
    ASSERT(aUserName != nullptr && aUserName[0] != '\0');
    ASSERT(aPassword != nullptr);

    if (mConnection.isOpen())
    {
        mConnection.close();
    }

    mConnection.setHostName(aHost);
    mConnection.setDatabaseName(aDbName);
    mConnection.setUserName(aUserName);
    mConnection.setPassword(aPassword);

    return mConnection.open();
}

QString
Database :: getSqlCommand(const QSqlQuery& aQuery)
{
    QString cmd = aQuery.lastQuery();
    QMapIterator<QString, QVariant> it(aQuery.boundValues());

    while (it.hasNext())
    {
        it.next();
        cmd.replace(it.key(), it.value().toString());
    }

    return cmd;
}

err_t
Database :: authenticate(Client& aClient, const char* aAccount, const char* aPassword)
{
    ASSERT_ERR(aAccount != nullptr && aAccount[0] != '\0', ERROR_INVALID_PARAMETER);
    ASSERT_ERR(aPassword != nullptr && aPassword[0] != '\0', ERROR_INVALID_PARAMETER);

    const char* cmd = "SELECT `id`, `password` FROM `account` WHERE `name` = :name";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":name", aAccount);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        if (query.size() == 1)
        {
            query.next(); // get the first result...

            int32_t accountID = (int32_t)query.value(0).toInt();
            QString password = query.value(1).toString();
            if (password.compare(aPassword) == 0)
            {
                aClient.mAccountID = accountID;
                LOG(DBG, "Account ID of %s is %d", aAccount, accountID);
            }
            else
            {
                // the Account/Password pair is not found
                err = ERROR_NOT_FOUND;
            }


        }
        else
        {
            LOG(ERROR, "The cmd should return only one result, not %d", query.size());
            err = ERROR_NOT_FOUND; // suppose the account is not found
        }
    }
    else
    {
        LOG(ERROR, "Failed to execute the following cmd : \"%s\"\nError: %s",
            cmd, qPrintable(query.lastError().text()));
        err = ERROR_EXEC_FAILED;
    }

    return err;
}

err_t
Database :: createPlayer(Client& aClient, const char* aName,
                         uint16_t aLook, uint16_t aProfession)
{
    ASSERT_ERR(&aClient != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(aName != nullptr && aName[0] != '\0', ERROR_INVALID_PARAMETER);

    const char* cmd = "INSERT INTO `user` (`account_id`, `name`, `lookface`, `profession`, "
                      "`force`, `dexterity`, `health`, `soul`, `life`, `mana`) VALUES "
                      "(:account_id, :name, :lookface, :profession, :force, :dexterity, "
                      ":health, :soul, :life, :mana)";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":account_id", aClient.getAccountID());
    query.bindValue(":name", aName);

    // TODO
//    Byte Face = 67;
//    if (pMsg->Look / 1000 == 2)
//        Face = 201;
    query.bindValue(":lookface", (Player::FACE_INTERN * 10000) + aLook);

    uint16_t force = 0, dexterity = 0, health = 0, soul = 0;

    query.bindValue(":force", force);
    query.bindValue(":dexterity", dexterity);
    query.bindValue(":health", health);
    query.bindValue(":soul", soul);

    query.bindValue(":life", 0);
    query.bindValue(":mana", 0);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (!query.exec())
    {
        LOG(ERROR, "Failed to execute the following cmd : \"%s\"\nError: %s",
            cmd, qPrintable(query.lastError().text()));
        err = ERROR_EXEC_FAILED;
    }

    return err;
}

err_t
Database :: getPlayerInfo(Client& aClient)
{
    ASSERT_ERR(&aClient != nullptr, ERROR_INVALID_REFERENCE);

    const char* cmd = "SELECT `id`, `name`, `mate`, `lookface`, `hair`, `money`, `money_saved`, "
                      "`level`, `exp`, `force`, `dexterity`, `health`, `soul`, `add_points`, `life`, "
                      "`mana`, `profession`, `pk`, `virtue`, `metempsychosis`, `record_map`, "
                      "`record_x`, `record_y` FROM `user` WHERE `account_id` = :account_id";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":account_id", aClient.getAccountID());

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        if (query.size() == 1)
        {
            query.next(); // get the first result...

            uint32_t uid = (uint32_t)query.value(0).toInt();
            Player* player = new Player(aClient, uid);

            player->mName = query.value(1).toString().toStdString();
            player->mMate = query.value(2).toString().toStdString();
            player->mLook = (uint32_t)query.value(3).toInt();
            player->mHair = (uint16_t)query.value(4).toInt();

            player->mMoney = (uint32_t)query.value(5).toInt();
            // money_saved

            player->mProfession = (uint8_t)query.value(16).toInt();
            player->mLevel = (uint8_t)query.value(7).toInt();
            player->mExp = (uint32_t)query.value(8).toInt();
            player->mMetempsychosis = (uint8_t)query.value(19).toInt();

            player->mForce = (uint16_t)query.value(9).toInt();
            player->mDexterity = (uint16_t)query.value(10).toInt();
            player->mHealth = (uint16_t)query.value(11).toInt();
            player->mSoul = (uint16_t)query.value(12).toInt();
            player->mAddPoints = (uint16_t)query.value(13).toInt();

            player->mCurHP = (uint16_t)query.value(14).toInt();
            player->mCurMP = (uint16_t)query.value(15).toInt();

            player->mPkPoints = (int16_t)query.value(17).toInt();
            player->mVirtue = (int32_t)query.value(18).toInt();

            player->mMapId = (uint32_t)query.value(20).toInt();
            player->mPosX = (uint16_t)query.value(21).toInt();
            player->mPosY = (uint16_t)query.value(22).toInt();

            player->mPrevMap = player->mMapId;
            player->mPosX = player->mPosX;
            player->mPosY = player->mPosY;

            aClient.mCharacter = player->getName();
            aClient.mPlayer = player;

            player = nullptr;
        }
        else if (query.size() == 0)
        {
            // not found, ignore... will create a new player
        }
        else
        {
            LOG(ERROR, "The cmd should return only one result, not %d", query.size());
            err = ERROR_BAD_LENGTH;
        }
    }
    else
    {
        LOG(ERROR, "Failed to execute the following cmd : \"%s\"\nError: %s",
            cmd, qPrintable(query.lastError().text()));
        err = ERROR_EXEC_FAILED;
    }

    return err;
}

err_t
Database :: savePlayer(Client& aClient)
{
    ASSERT_ERR(&aClient != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(aClient.getPlayer() != nullptr, ERROR_INVALID_POINTER);

    const char* cmd = "UPDATE `user` SET `mate` = :mate, `lookface` = :lookface, `hair` = :hair, "
                      "`money` = :money, `money_saved` = :money_saved, "
                      "`level` = :level, `exp` = :exp, "
                      "`force` = :force, `dexterity` = :dexterity, `health` = :health, "
                      "`soul` = :soul, `add_points` = :add_points, `life` = :life, `mana` = :mana, "
                      "`pk` = :pk, `virtue` = :virtue, `metempsychosis` = :metempsychosis, "
                      "`record_map` = :record_map, `record_x` = :record_x, `record_y` = :record_y "
                      "WHERE `account_id` = :account_id";

    err_t err = ERROR_SUCCESS;

    Player& player = *aClient.getPlayer();

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":account_id", aClient.getAccountID());

    query.bindValue(":mate", player.getMate());
    query.bindValue(":lookface", player.getLook());
    query.bindValue(":hair", player.getHair());

    query.bindValue(":money", player.getMoney());
    query.bindValue(":money_saved", 0); // TODO

    query.bindValue(":level", player.getLevel());
    query.bindValue(":exp", player.getExp());
    query.bindValue(":metempsychosis", player.getMetempsychosis());

    query.bindValue(":force", player.getForce());
    query.bindValue(":dexterity", player.getDexterity());
    query.bindValue(":health", player.getHealth());
    query.bindValue(":soul", player.getSoul());
    query.bindValue(":add_points", player.getAddPoints());

    query.bindValue(":life", player.getCurHP());
    query.bindValue(":mana", player.getCurMP());

    query.bindValue(":pk", player.getPkPoints());
    query.bindValue(":virtue", player.getVirtue());

    query.bindValue(":record_map", player.getMapId());
    query.bindValue(":record_x", player.getPosX());
    query.bindValue(":record_y", player.getPosY());

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (!query.exec())
    {
        LOG(ERROR, "Failed to execute the following cmd : \"%s\"\nError: %s",
            cmd, qPrintable(query.lastError().text()));
        err = ERROR_EXEC_FAILED;
    }

    return err;
}

err_t
Database :: loadAllNPCs()
{
    const char* cmd = "SELECT * FROM `npc`";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        World& world = World::getInstance();
        while (ERROR_SUCCESS == err && query.next())
        {
            Npc* npc = new Npc(
                           (int32_t)query.value(0).toInt(),
                           nullptr,
                           (uint8_t)query.value(2).toInt(),
                           (int16_t)query.value(3).toInt(),
                           (uint32_t)query.value(4).toInt(),
                           (uint16_t)query.value(5).toInt(),
                           (uint16_t)query.value(6).toInt(),
                           (uint8_t)query.value(9).toInt(),
                           (uint8_t)query.value(10).toInt());

            ASSERT(npc != nullptr);
            ASSERT(world.AllNPCs.find(npc->getUID()) == world.AllNPCs.end());

            world.AllNPCs[npc->getUID()] = npc;
        }

        if (IS_SUCCESS(err))
        {
            fprintf(stdout, "Loaded all NPCs.\n");
            LOG(INFO, "Loaded all NPCs.");
        }
    }
    else
    {
        LOG(ERROR, "Failed to execute the following cmd : \"%s\"\nError: %s",
            cmd, qPrintable(query.lastError().text()));
        err = ERROR_EXEC_FAILED;
    }

    return err;
}

err_t
Database :: loadAllMaps()
{
    const char* cmd = "SELECT `id`, `doc_id`, `type`, `weather`, `portal_x`, `portal_y`, `reborn_map`, `reborn_portal`, `light` FROM `map`";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        MapManager& mgr = MapManager::getInstance();
        while (ERROR_SUCCESS == err && query.next())
        {
            GameMap::Info* info = new GameMap::Info();
            uint32_t uid = (uint32_t)query.value(0).toInt();

            info->OwnerUID = 0;
            info->DocID = (uint16_t)query.value(1).toInt();
            info->Type = (uint32_t)query.value(2).toInt();
            // TODO: weather
            info->PortalX = (uint16_t)query.value(4).toInt();
            info->PortalY = (uint16_t)query.value(5).toInt();
            info->RebornMap = (uint32_t)query.value(6).toInt();
            info->RebornPortal = (int32_t)query.value(7).toInt();
            info->Light = (uint32_t)query.value(8).toInt();

            ASSERT(info != nullptr);
            DOIF(err, mgr.createMap(uid, &info));

            SAFE_DELETE(info);
        }

        if (IS_SUCCESS(err))
        {
            fprintf(stdout, "Loaded all maps.\n");
            LOG(INFO, "Loaded all maps.");
        }
    }
    else
    {
        LOG(ERROR, "Failed to execute the following cmd : \"%s\"\nError: %s",
            cmd, qPrintable(query.lastError().text()));
        err = ERROR_EXEC_FAILED;
    }

    return err;
}

err_t
Database :: loadAllItems()
{
    const char* cmd = "SELECT * FROM `itemtype`";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        World& world = World::getInstance();
        while (ERROR_SUCCESS == err && query.next())
        {
            Item::Info* item = new Item::Info();
            ASSERT(item != nullptr);

            item->Id = (uint32_t)query.value(0).toInt();
            item->Name = query.value(1).toString().toStdString();
            item->ReqForce = (uint8_t)query.value(2).toInt();
            item->ReqWeaponSkill = (uint8_t)query.value(3).toInt();
            item->ReqLevel = (uint8_t)query.value(4).toInt();
            item->ReqSex = (uint8_t)query.value(5).toInt();
            item->ReqForce = (uint16_t)query.value(6).toInt();
            item->ReqSpeed = (uint16_t)query.value(7).toInt();
            item->ReqHealth = (uint16_t)query.value(8).toInt();
            item->ReqSoul = (uint16_t)query.value(9).toInt();
            item->Monopoly = (uint8_t)query.value(10).toInt();
            item->Weight = (uint16_t)query.value(11).toInt();
            item->Price = (uint32_t)query.value(12).toInt();
            item->Task = (uint32_t)query.value(13).toInt();
            item->MaxAtk = (uint16_t)query.value(14).toInt();
            item->MinAtk = (uint16_t)query.value(15).toInt();
            item->Defense = (int16_t)query.value(16).toInt();
            item->Dexterity = (int16_t)query.value(17).toInt();
            item->Dodge = (int16_t)query.value(18).toInt();
            item->Life = (int16_t)query.value(19).toInt();
            item->Mana = (int16_t)query.value(20).toInt();
            item->Amount = (uint16_t)query.value(21).toInt();
            item->AmountLimit = (uint16_t)query.value(22).toInt();
            item->Status = (uint8_t)query.value(23).toInt();
            item->Gem1 = (uint8_t)query.value(24).toInt();
            item->Gem2 = (uint8_t)query.value(25).toInt();
            item->Magic1 = (uint8_t)query.value(26).toInt();
            item->Magic2 = (uint8_t)query.value(27).toInt();
            item->Magic3 = (uint8_t)query.value(28).toInt();
            item->MagicAtk = (uint16_t)query.value(29).toInt();
            item->MagicDef = (uint16_t)query.value(30).toInt();
            item->AtkRange = (uint16_t)query.value(31).toInt();
            item->AtkSpeed = (uint16_t)query.value(32).toInt();

            // TODO: add somewhere...
        }

        if (IS_SUCCESS(err))
        {
            fprintf(stdout, "Loaded all items.\n");
            LOG(INFO, "Loaded all items.");
        }
    }
    else
    {
        LOG(ERROR, "Failed to execute the following cmd : \"%s\"\nError: %s",
            cmd, qPrintable(query.lastError().text()));
        err = ERROR_EXEC_FAILED;
    }

    return err;
}
