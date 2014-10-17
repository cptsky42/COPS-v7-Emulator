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
#include "weaponskill.h"
#include "magic.h"
#include "npc.h"
#include "npctask.h"
#include "itemtask.h"
#include "generator.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlResult>
#include <QtSql/QSqlDriver>
#include <QVariant>
#include <QSqlError>
#include <QDirIterator>
#include <QFileInfo>
#include <QThread>

using namespace std;

/* static */
Database* Database::sInstance = nullptr;

/* static */
const Database&
Database :: getInstance()
{
    static volatile long protect = 0;

    if (sInstance == nullptr)
    {
        if (1 == atomic_inc(&protect))
        {
            // create the instance
            sInstance = new Database();
        }
        else
        {
            while (sInstance == nullptr)
                QThread::yieldCurrentThread();
        }
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

    for (map<uint32_t, Item::Info*>::iterator
            it = mAllItems.begin(), end = mAllItems.end();
         it != end; ++it)
    {
        Item::Info* info = it->second;
        SAFE_DELETE(info);
    }
    mAllItems.clear();

    for (map<uint32_t, Magic::Info*>::iterator
            it = mAllMagics.begin(), end = mAllMagics.end();
         it != end; ++it)
    {
        Magic::Info* info = it->second;
        SAFE_DELETE(info);
    }
    mAllMagics.clear();

    for (map<uint32_t, Monster::Info*>::iterator
            it = mAllMonsters.begin(), end = mAllMonsters.end();
         it != end; ++it)
    {
        Monster::Info* info = it->second;
        SAFE_DELETE(info);
    }
    mAllMonsters.clear();
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

    if (mConnection.driver() != nullptr &&
        !mConnection.driver()->hasFeature(QSqlDriver::LastInsertId))
    {
        fprintf(stdout, "Warning ! %s driver doesn't have feature \"LastInsertId\".",
                qPrintable(mConnection.driverName()));
        LOG(WARN, "%s driver doesn't have feature \"LastInsertId\".",
            qPrintable(mConnection.driverName()));
    }

    return mConnection.open();
}

QString
Database :: getSqlCommand(const QSqlQuery& aQuery) const
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
Database :: authenticate(Client& aClient, const char* aAccount, const char* aPassword) const
{
    ASSERT_ERR(aAccount != nullptr && aAccount[0] != '\0', ERROR_INVALID_PARAMETER);
    ASSERT_ERR(aPassword != nullptr && aPassword[0] != '\0', ERROR_INVALID_PARAMETER);

    static const char cmd[] = "SELECT `id`, `password` FROM `account` WHERE `name` = :name";

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
                         uint16_t aLook, uint16_t aProfession) const
{
    ASSERT_ERR(&aClient != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(aName != nullptr && aName[0] != '\0', ERROR_INVALID_PARAMETER);

    static const char cmd[] =
            "INSERT INTO `user` (`account_id`, `name`, `lookface`, `profession`, "
            "`force`, `dexterity`, `health`, `soul`, `life`, `mana`) VALUES "
            "(:account_id, :name, :lookface, :profession, :force, :dexterity, "
            ":health, :soul, :life, :mana)";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":account_id", aClient.getAccountID());
    query.bindValue(":name", aName);

    uint16_t face = Player::FACE_MALE;
    if (aLook / 1000 == Player::SEX_FEMALE)
        face = Player::FACE_FEMALE;
    query.bindValue(":lookface", (face * 10000) + aLook);
    query.bindValue(":profession", aProfession);

    uint16_t force = 0, dexterity = 0, health = 0, soul = 0;
    getPointAllot(force, dexterity, health, soul, 1, aProfession);

    query.bindValue(":force", force);
    query.bindValue(":dexterity", dexterity);
    query.bindValue(":health", health);
    query.bindValue(":soul", soul);

    query.bindValue(":life", (force * 3) + (dexterity * 3) + (health * 24) + (soul * 3));
    query.bindValue(":mana", (soul * 5));

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
Database :: getPlayerInfo(Client& aClient) const
{
    ASSERT_ERR(&aClient != nullptr, ERROR_INVALID_REFERENCE);

    static const char cmd[] =
            "SELECT * FROM `user` WHERE `account_id` = :account_id";

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

            uint32_t uid = (uint32_t)query.value(Player::SQLDATA_ID).toUInt();
            Player* player = new Player(aClient, uid);

            player->mName = query.value(Player::SQLDATA_NAME).toString().toStdString();
            player->mMate = query.value(Player::SQLDATA_MATE).toString().toStdString();
            player->mLook = (uint32_t)query.value(Player::SQLDATA_LOOKFACE).toUInt();
            player->mHair = (uint16_t)query.value(Player::SQLDATA_HAIR).toUInt();

            player->mMoney = (uint32_t)query.value(Player::SQLDATA_MONEY).toUInt();
            // money_saved
            player->mCPs = (uint32_t)query.value(Player::SQLDATA_CONQUER_POINTS).toUInt();

            player->mProfession = (uint8_t)query.value(Player::SQLDATA_PROFESSION).toUInt();
            player->mLevel = (uint8_t)query.value(Player::SQLDATA_LEVEL).toUInt();
            player->mExp = (uint64_t)query.value(Player::SQLDATA_EXP).toULongLong();
            player->mMetempsychosis = (uint8_t)query.value(Player::SQLDATA_METEMPSYCHOSIS).toUInt();

            player->mForce = (uint16_t)query.value(Player::SQLDATA_FORCE).toUInt();
            player->mDexterity = (uint16_t)query.value(Player::SQLDATA_DEXTERITY).toUInt();
            player->mHealth = (uint16_t)query.value(Player::SQLDATA_HEALTH).toUInt();
            player->mSoul = (uint16_t)query.value(Player::SQLDATA_SOUL).toUInt();
            player->mAddPoints = (uint16_t)query.value(Player::SQLDATA_ADD_POINTS).toUInt();

            player->mCurHP = (uint16_t)query.value(Player::SQLDATA_LIFE).toUInt();
            player->mCurMP = (uint16_t)query.value(Player::SQLDATA_MANA).toUInt();

            player->mPkPoints = (int16_t)query.value(Player::SQLDATA_PK_POINTS).toInt();
            player->mVirtue = (int32_t)query.value(Player::SQLDATA_VIRTUE).toInt();

            player->mMapId = (uint32_t)query.value(Player::SQLDATA_RECORD_MAP).toUInt();
            player->mPosX = (uint16_t)query.value(Player::SQLDATA_RECORD_X).toUInt();
            player->mPosY = (uint16_t)query.value(Player::SQLDATA_RECORD_Y).toUInt();

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
Database :: getPlayerItems(Player& aPlayer) const
{
    ASSERT_ERR(&aPlayer != nullptr, ERROR_INVALID_REFERENCE);

    static const char cmd[] =
            "SELECT * FROM `item` WHERE `player_id` = :player_id";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":player_id", aPlayer.getUID());

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        while (ERROR_SUCCESS == err && query.next())
        {
            Item* item = nullptr;
            err = Item::createItem(&item, query);

            if (IS_SUCCESS(err))
            {
                if (item->getOwner() != nullptr &&
                    item->getOwner()->getUID() == aPlayer.getUID())
                {
                    if (Item::POS_INVENTORY == item->getPosition())
                    {
                        ASSERT(aPlayer.mInventory.size() < Player::MAX_INVENTORY_SIZE);

                        aPlayer.mInventoryMutex.lock();
                        aPlayer.mInventory.insert(aPlayer.mInventory.end(),
                                                  pair<uint32_t, Item*>(item->getUID(), item));
                        aPlayer.mInventoryMutex.unlock();
                    }
                    else if (Item::MAX_EQUIPMENT > item->getPosition())
                    {
                        aPlayer.mEquipment[item->getPosition()] = item;
                    }
                }
            }
            else
            {
                LOG(WARN, "Failed to create item %u for player %u. Ignoring.",
                    (uint32_t)query.value(Item::SQLDATA_ID).toUInt(), aPlayer.getUID());
                err = ERROR_SUCCESS;
            }
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
Database :: getPlayerWeaponSkills(Player& aPlayer) const
{
    ASSERT_ERR(&aPlayer != nullptr, ERROR_INVALID_REFERENCE);

    static const char cmd[] =
            "SELECT * FROM `weaponskill` WHERE `owner_id` = :owner_id";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":owner_id", aPlayer.getUID());

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        while (ERROR_SUCCESS == err && query.next())
        {
            WeaponSkill* skill = nullptr;
            err = WeaponSkill::createSkill(&skill, query);

            if (IS_SUCCESS(err))
            {
                if (skill->getPlayer().getUID() == aPlayer.getUID())
                {
                    aPlayer.mWeaponSkillsMutex.lock();
                    aPlayer.mWeaponSkills.insert(aPlayer.mWeaponSkills.end(),
                                                 pair<uint16_t, WeaponSkill*>(skill->getType(), skill));
                    aPlayer.mWeaponSkillsMutex.unlock();
                }
            }
            else
            {
                LOG(WARN, "Failed to create weapon skill %u for player %u. Ignoring.",
                    (uint32_t)query.value(WeaponSkill::SQLDATA_ID).toUInt(), aPlayer.getUID());
                err = ERROR_SUCCESS;
            }
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
Database :: getPlayerMagicSkills(Player& aPlayer) const
{
    ASSERT_ERR(&aPlayer != nullptr, ERROR_INVALID_REFERENCE);

    static const char cmd[] =
            "SELECT * FROM `magic` WHERE `owner_id` = :owner_id";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":owner_id", aPlayer.getUID());

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        while (ERROR_SUCCESS == err && query.next())
        {
            Magic* magic = nullptr;
            err = Magic::createMagic(&magic, query);

            if (IS_SUCCESS(err))
            {
                if (magic->getPlayer().getUID() == aPlayer.getUID())
                {
                    aPlayer.mMagicsMutex.lock();
                    aPlayer.mMagics.insert(aPlayer.mMagics.end(),
                                           pair<uint16_t, Magic*>(magic->getType(), magic));
                    aPlayer.mMagicsMutex.unlock();
                }
            }
            else
            {
                LOG(WARN, "Failed to create magic skill %u for player %u. Ignoring.",
                    (uint32_t)query.value(Magic::SQLDATA_ID).toUInt(), aPlayer.getUID());
                err = ERROR_SUCCESS;
            }
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
Database :: savePlayer(Client& aClient) const
{
    ASSERT_ERR(&aClient != nullptr, ERROR_INVALID_REFERENCE);
    ASSERT_ERR(aClient.getPlayer() != nullptr, ERROR_INVALID_POINTER);

    static const char cmd[] =
            "UPDATE `user` SET `mate` = :mate, `lookface` = :lookface, `hair` = :hair, "
            "`money` = :money, `cps` = :cps, "
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
    // TODO add saved money
    query.bindValue(":cps", player.getCPs());

    query.bindValue(":level", player.getLevel());
    query.bindValue(":exp", (qulonglong)player.getExp());
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
Database :: saveItem(const Item& aItem) const
{
    ASSERT_ERR(&aItem != nullptr, ERROR_INVALID_REFERENCE);

    static const char cmd[] =
            "UPDATE `item` SET `type` = :type, `owner_id` = :owner_id, `player_id` = :player_id, "
            "`amount` = :amount__, `amount_limit` = :amount_limit, "
            "`ident` = :ident, `position` = :position, "
            "`gem1` = :gem1, `gem2` = :gem2, `magic1` = :magic1, `magic2` = :magic2, `magic3` = :magic3, "
            "`restrain` = :restrain, `bless` = :bless, `enchant` = :enchant, "
            "`suspicious` = :suspicious, `locked` = :locked, `color` = :color "
            "WHERE `id` = :uid";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":uid", aItem.getUID());

    query.bindValue(":type", aItem.getType());
    query.bindValue(":owner_id", aItem.getOwner() != nullptr ? aItem.getOwner()->getUID() : 0);
    query.bindValue(":player_id", aItem.getPlayer() != nullptr ? aItem.getPlayer()->getUID() : 0);

    query.bindValue(":amount__", aItem.getAmount());
    query.bindValue(":amount_limit", aItem.getAmountLimit());

    query.bindValue(":ident", aItem.getIdent());
    query.bindValue(":position", aItem.getPosition());

    query.bindValue(":gem1", aItem.getGem1());
    query.bindValue(":gem2", aItem.getGem2());
    query.bindValue(":magic1", aItem.getMagic1());
    query.bindValue(":magic2", aItem.getMagic2());
    query.bindValue(":magic3", aItem.getMagic3());
    query.bindValue(":restrain", aItem.getRestrain());
    query.bindValue(":bless", aItem.getBless());
    query.bindValue(":enchant", aItem.getEnchant());

    query.bindValue(":suspicious", aItem.isSuspicious());
    query.bindValue(":locked", aItem.isLocked());

    query.bindValue(":color", aItem.getColor());

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
Database :: createItem(Item** aOutItem, const Item::Info& aInfo,
                       const Player& aPlayer,
                       uint16_t aAmount, uint16_t aAmountLimit,
                       uint8_t aIdent, Item::Position aPosition) const
{
    ASSERT_ERR(aOutItem != nullptr && *aOutItem == nullptr, ERROR_INVALID_POINTER);
    ASSERT_ERR(&aInfo != nullptr, ERROR_INVALID_REFERENCE);

    static const char cmd[] =
            "INSERT INTO `item` (`type`, `owner_id`, `player_id`, "
            "`amount`, `amount_limit`, `ident`, `position`) VALUES "
            "(:type, :owner_id, :player_id, :amount__, :amount_limit, "
            ":ident, :position)";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":type", aInfo.Id);
    query.bindValue(":owner_id", aPlayer.getUID());
    query.bindValue(":player_id", aPlayer.getUID());
    query.bindValue(":amount__", aAmount);
    query.bindValue(":amount_limit", aAmountLimit);
    query.bindValue(":ident", aIdent);
    query.bindValue(":position", (uint8_t)aPosition);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        uint32_t uid = (uint32_t)query.lastInsertId().toUInt();
        *aOutItem = new Item(uid, aInfo);
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
Database :: eraseItem(const Item& aItem) const
{
    ASSERT_ERR(&aItem != nullptr, ERROR_INVALID_REFERENCE);

    static const char cmd[] =
            "DELETE FROM `item` WHERE `id` = :id";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":id", aItem.getUID());

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
Database :: getRequiredExp(uint64_t aOutExp, uint8_t aLevel) const
{
    static const char cmd[] = "SELECT `exp` FROM `levelexp` WHERE `level` = :level";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":level", aLevel);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        if (query.size() == 1)
        {
            query.next(); // get the first result...

            aOutExp = (uint64_t)query.value(0).toULongLong();
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
Database :: getUpLvlTime(uint32_t aOutTime, uint8_t aLevel) const
{
    static const char cmd[] = "SELECT `up_lvl_time` FROM `levelexp` WHERE `level` = :level";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":level", aLevel);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        if (query.size() == 1)
        {
            query.next(); // get the first result...

            aOutTime = (uint32_t)query.value(0).toUInt();
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
Database :: getPointAllot(uint16_t& aOutForce, uint16_t& aOutSpeed, uint16_t& aOutHealth, uint16_t aOutSoul,
                          uint8_t aLevel, uint8_t aProfession) const
{
    static const char cmd[] = "SELECT `force`, `speed`, `health`, `soul` FROM `point_allot` WHERE `profession` = :profession AND `level` = :level";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":profession", aProfession / 10);
    query.bindValue(":level", aLevel);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        if (query.size() == 1)
        {
            query.next(); // get the first result...

            aOutForce = (uint16_t)query.value(0).toUInt();
            aOutSpeed = (uint16_t)query.value(1).toUInt();
            aOutHealth = (uint16_t)query.value(2).toUInt();
            aOutSoul = (uint16_t)query.value(3).toUInt();
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
Database :: loadAllMaps()
{
    static const char cmd[] = "SELECT `id`, `doc_id`, `type`, `weather`, `portal_x`, `portal_y`, `reborn_map`, `reborn_portal`, `light` FROM `map`";

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
            uint32_t uid = (uint32_t)query.value(0).toUInt();

            info->OwnerUID = 0;
            info->DocID = (uint16_t)query.value(1).toUInt();
            info->Type = (uint32_t)query.value(2).toUInt();
            info->Weather = (uint32_t)query.value(3).toUInt();
            info->PortalX = (uint16_t)query.value(4).toUInt();
            info->PortalY = (uint16_t)query.value(5).toUInt();
            info->RebornMap = (uint32_t)query.value(6).toUInt();
            info->RebornPortal = (int32_t)query.value(7).toInt();
            info->Light = (uint32_t)query.value(8).toUInt();

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
Database :: getPasswayInfo(uint32_t& aOutMapId, uint16_t& aOutPosX, uint16_t& aOutPosY,
                           uint32_t aMapId, uint8_t aIndex) const
{
    static const char cmd1[] = "SELECT `portal_mapid`, `portal_idx` FROM `passway` WHERE `mapid` = :mapid AND `passway_idx` = :index";
    static const char cmd2[] = "SELECT `portal_x`, `portal_y` FROM `portal` WHERE `mapid` = :mapid AND `idx` = :index";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd1);
    query.bindValue(":mapid", aMapId);
    query.bindValue(":index", aIndex);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        if (query.size() == 1)
        {
            query.next(); // get the first result...

            uint32_t mapId = (uint32_t)query.value(0).toUInt();
            uint8_t index = (uint8_t)query.value(1).toUInt();

            // prepare the new query
            query.prepare(cmd2);
            query.bindValue(":mapid", mapId);
            query.bindValue(":index", index);

            LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

            if (query.exec())
            {
                if (query.size() == 1)
                {
                    query.next(); // get the first result...

                    aOutMapId = mapId;
                    aOutPosX = (uint16_t)query.value(0).toUInt();
                    aOutPosY = (uint16_t)query.value(1).toUInt();

                    LOG(INFO, "Passway %u at %u will transport the player to %u (%u, %u).",
                        aIndex, aMapId, aOutMapId, aOutPosX, aOutPosY);
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
                    cmd2, qPrintable(query.lastError().text()));
                err = ERROR_EXEC_FAILED;
            }
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
            cmd1, qPrintable(query.lastError().text()));
        err = ERROR_EXEC_FAILED;
    }

    return err;
}

err_t
Database :: loadAllItems()
{
    static const char cmd[] = "SELECT * FROM `itemtype`";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        World& world = World::getInstance();
        while (ERROR_SUCCESS == err && query.next())
        {
            Item::Info* info = new Item::Info();
            ASSERT(info != nullptr);

            info->Id = (uint32_t)query.value(Item::SQLDATA_INFO_ID).toUInt();
            info->Name = query.value(Item::SQLDATA_INFO_NAME).toString().toStdString();
            info->ReqProf = (uint8_t)query.value(Item::SQLDATA_INFO_REQ_PROF).toUInt();
            info->ReqWeaponSkill = (uint8_t)query.value(Item::SQLDATA_INFO_REQ_WEAPONSKILL).toUInt();
            info->ReqLevel = (uint8_t)query.value(Item::SQLDATA_INFO_REQ_LEVEL).toUInt();
            info->ReqSex = (uint8_t)query.value(Item::SQLDATA_INFO_REQ_SEX).toUInt();
            info->ReqForce = (uint16_t)query.value(Item::SQLDATA_INFO_REQ_FORCE).toUInt();
            info->ReqSpeed = (uint16_t)query.value(Item::SQLDATA_INFO_REQ_SPEED).toUInt();
            info->ReqHealth = (uint16_t)query.value(Item::SQLDATA_INFO_REQ_HEALTH).toUInt();
            info->ReqSoul = (uint16_t)query.value(Item::SQLDATA_INFO_REQ_SOUL).toUInt();
            info->Monopoly = (uint8_t)query.value(Item::SQLDATA_INFO_MONOPOLY).toUInt();
            info->Weight = (uint16_t)query.value(Item::SQLDATA_INFO_WEIGHT).toUInt();
            info->Price = (uint32_t)query.value(Item::SQLDATA_INFO_PRICE).toUInt();
            info->Task = nullptr;
            info->MaxAtk = (uint16_t)query.value(Item::SQLDATA_INFO_MAX_ATK).toUInt();
            info->MinAtk = (uint16_t)query.value(Item::SQLDATA_INFO_MIN_ATK).toUInt();
            info->Defense = (int16_t)query.value(Item::SQLDATA_INFO_DEFENSE).toInt();
            info->Dexterity = (int16_t)query.value(Item::SQLDATA_INFO_DEXTERITY).toInt();
            info->Dodge = (int16_t)query.value(Item::SQLDATA_INFO_DODGE).toInt();
            info->Life = (int16_t)query.value(Item::SQLDATA_INFO_LIFE).toInt();
            info->Mana = (int16_t)query.value(Item::SQLDATA_INFO_MANA).toInt();
            info->Amount = (uint16_t)query.value(Item::SQLDATA_INFO_AMOUNT).toUInt();
            info->AmountLimit = (uint16_t)query.value(Item::SQLDATA_INFO_AMOUNT_LIMIT).toUInt();
            info->Ident = (uint8_t)query.value(Item::SQLDATA_INFO_IDENT).toUInt();
            info->Gem1 = (uint8_t)query.value(Item::SQLDATA_INFO_GEM1).toUInt();
            info->Gem2 = (uint8_t)query.value(Item::SQLDATA_INFO_GEM2).toUInt();
            info->Magic1 = (uint8_t)query.value(Item::SQLDATA_INFO_MAGIC1).toUInt();
            info->Magic2 = (uint8_t)query.value(Item::SQLDATA_INFO_MAGIC2).toUInt();
            info->Magic3 = (uint8_t)query.value(Item::SQLDATA_INFO_MAGIC3).toUInt();
            info->MagicAtk = (uint16_t)query.value(Item::SQLDATA_INFO_MAGIC_ATK).toUInt();
            info->MagicDef = (uint16_t)query.value(Item::SQLDATA_INFO_MAGIC_DEF).toUInt();
            info->AtkRange = (uint16_t)query.value(Item::SQLDATA_INFO_ATK_RANGE).toUInt();
            info->AtkSpeed = (uint16_t)query.value(Item::SQLDATA_INFO_ATK_SPEED).toUInt();
            info->FrayMode = (uint8_t)query.value(Item::SQLDATA_INFO_FRAY_MODE).toUInt();
            info->RepairMode = (uint8_t)query.value(Item::SQLDATA_INFO_REPAIR_MODE).toUInt();
            info->TypeMask = (uint8_t)query.value(Item::SQLDATA_INFO_TYPE_MASK).toUInt();
            info->CPs = (uint32_t)query.value(Item::SQLDATA_INFO_CPS).toUInt();

            ASSERT(info != nullptr);
            ASSERT(mAllItems.find(info->Id) == mAllItems.end());

            uint32_t taskId = (uint32_t)query.value(Item::SQLDATA_INFO_TASK).toUInt();
            if (taskId != 0)
            {
                Script* task = nullptr;
                if (world.queryTask(&task, taskId))
                    info->Task = dynamic_cast<ItemTask*>(task);
                else
                    LOG(WARN, "Task %u is not loaded but needed by %u.",
                        taskId, info->Id);
            }

            mAllItems[info->Id] = info;
            info = nullptr;

            SAFE_DELETE(info);
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

err_t
Database :: getItemInfo(const Item::Info** aOutInfo, uint32_t aId) const
{
    ASSERT_ERR(aOutInfo != nullptr && *aOutInfo == nullptr, ERROR_INVALID_POINTER);

    err_t err = ERROR_SUCCESS;

    map<uint32_t, Item::Info*>::const_iterator it = mAllItems.find(aId);
    if (it != mAllItems.end())
    {
        *aOutInfo = it->second;
    }
    else
    {
        LOG(WARN, "Could not find the info of the item %u.", aId);
        err = ERROR_NOT_FOUND;
    }

    return err;
}

err_t
Database :: getItemFromShop(const Item::Info** aOutInfo, uint8_t& aOutMoneyType,
                            uint32_t aShopId, uint32_t aItemId) const
{
    static const char cmd[] = "SELECT `id`, `moneytype` FROM `goods` "
                              "WHERE `ownerid` = :ownerid AND `itemtype` = :itemtype";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);
    query.bindValue(":ownerid", aShopId);
    query.bindValue(":itemtype", aItemId);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        if (query.size() == 1)
        {
            query.next(); // get the first result...

            aOutMoneyType = (uint8_t)query.value(1).toUInt();
            err = getItemInfo(aOutInfo, aItemId);
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
Database :: loadAllMagics()
{
    static const char cmd[] = "SELECT * FROM `magictype`";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        while (ERROR_SUCCESS == err && query.next())
        {
            Magic::Info* info = new Magic::Info();
            ASSERT(info != nullptr);

            info->Id = (uint32_t)query.value(Magic::SQLDATA_INFO_ID).toUInt();
            info->Type = (uint16_t)query.value(Magic::SQLDATA_INFO_TYPE).toUInt();
            info->Sort = (uint8_t)query.value(Magic::SQLDATA_INFO_SORT).toUInt();
            info->Crime = query.value(Magic::SQLDATA_INFO_CRIME).toBool();
            info->Ground = query.value(Magic::SQLDATA_INFO_GROUND).toBool();
            info->Multi = query.value(Magic::SQLDATA_INFO_MULTI).toBool();
            info->Target = (uint32_t)query.value(Magic::SQLDATA_INFO_TARGET).toUInt();
            info->Level = (uint16_t)query.value(Magic::SQLDATA_INFO_LEVEL).toUInt();
            info->UseMP = (uint16_t)query.value(Magic::SQLDATA_INFO_USE_MP).toUInt();
            info->Power = (int32_t)query.value(Magic::SQLDATA_INFO_POWER).toInt();
            info->IntoneDuration = (uint32_t)query.value(Magic::SQLDATA_INFO_INTONE_DURATION).toUInt();
            info->Success = (uint8_t)query.value(Magic::SQLDATA_INFO_SUCCESS).toUInt();
            info->StepSecs = (uint32_t)query.value(Magic::SQLDATA_INFO_STEP_SECS).toUInt();
            info->Range = (uint8_t)query.value(Magic::SQLDATA_INFO_RANGE).toUInt();
            info->Distance = (uint8_t)query.value(Magic::SQLDATA_INFO_DISTANCE).toUInt();
            info->Status = (uint32_t)query.value(Magic::SQLDATA_INFO_STATUS).toUInt();
            info->ReqProf = (uint16_t)query.value(Magic::SQLDATA_INFO_REQ_PROF).toUInt();
            info->ReqExp = (uint32_t)query.value(Magic::SQLDATA_INFO_REQ_EXP).toUInt();
            info->ReqLevel = (uint8_t)query.value(Magic::SQLDATA_INFO_REQ_LEVEL).toUInt();
            info->UseXP = (uint8_t)query.value(Magic::SQLDATA_INFO_USE_XP).toUInt();
            info->WeaponSubtype = (uint16_t)query.value(Magic::SQLDATA_INFO_WEAPON_SUBTYPE).toUInt();
            info->ActiveTimes = (uint32_t)query.value(Magic::SQLDATA_INFO_ACTIVE_TIMES).toUInt();
            info->AutoActive = query.value(Magic::SQLDATA_INFO_AUTO_ACTIVE).toBool();
            info->FloorAttr = (uint32_t)query.value(Magic::SQLDATA_INFO_FLOOR_ATTR).toUInt();
            info->AutoLearn = query.value(Magic::SQLDATA_INFO_AUTO_LEARN).toBool();
            info->LearnLevel = (uint8_t)query.value(Magic::SQLDATA_INFO_LEARN_LEVEL).toUInt();
            info->DropWeapon = query.value(Magic::SQLDATA_INFO_DROP_WEAPON).toBool();
            info->UseEP = (uint8_t)query.value(Magic::SQLDATA_INFO_USE_EP).toUInt();
            info->WeaponHit = query.value(Magic::SQLDATA_INFO_WEAPON_HIT).toBool();
            info->UseItem = (uint32_t)query.value(Magic::SQLDATA_INFO_USE_ITEM).toUInt();
            // TODO: SQLDATA_INFO_NEXT_MAGIC,
            info->NextMagicDelay = (uint16_t)query.value(Magic::SQLDATA_INFO_NEXT_MAGIC_DELAY).toUInt();
            info->UseItemNum = (uint8_t)query.value(Magic::SQLDATA_INFO_USE_ITEM_NUM).toUInt();
            info->UsableInMarket = query.value(Magic::SQLDATA_INFO_USABLE_IN_MARKET).toBool();

            ASSERT(info != nullptr);
            ASSERT(mAllMagics.find(info->Id) == mAllMagics.end());

            mAllMagics[info->Id] = info;
            info = nullptr;

            SAFE_DELETE(info);
        }

        if (IS_SUCCESS(err))
        {
            fprintf(stdout, "Loaded all magics.\n");
            LOG(INFO, "Loaded all magics.");
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
Database :: getMagicInfo(const Magic::Info** aOutInfo, uint16_t aType, uint16_t aLevel) const
{
    ASSERT_ERR(aOutInfo != nullptr && *aOutInfo == nullptr, ERROR_INVALID_POINTER);

    err_t err = ERROR_SUCCESS;

    map<uint32_t, Magic::Info*>::const_iterator it = mAllMagics.find((aType * 10) + aLevel);
    if (it != mAllMagics.end())
    {
        *aOutInfo = it->second;
    }
    else
    {
        LOG(WARN, "Could not find the info of the magic skill %u (level %u).", aType, aLevel);
        err = ERROR_NOT_FOUND;
    }

    return err;
}

err_t
Database :: loadAllNPCs()
{
    static const char cmd[] = "SELECT * FROM `npc`";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        World& world = World::getInstance();
        MapManager& mgr = MapManager::getInstance();
        while (ERROR_SUCCESS == err && query.next())
        {
            Npc* npc = new Npc(
                           (int32_t)query.value(Npc::SQLDATA_ID).toInt(),
                           nullptr,
                           (uint8_t)query.value(Npc::SQLDATA_TYPE).toUInt(),
                           (uint16_t)query.value(Npc::SQLDATA_LOOKFACE).toUInt(),
                           (uint32_t)query.value(Npc::SQLDATA_MAPID).toUInt(),
                           (uint16_t)query.value(Npc::SQLDATA_CELLX).toUInt(),
                           (uint16_t)query.value(Npc::SQLDATA_CELLY).toUInt(),
                           (uint8_t)query.value(Npc::SQLDATA_BASE).toUInt(),
                           (uint8_t)query.value(Npc::SQLDATA_SORT).toUInt());

            ASSERT(npc != nullptr);
            ASSERT(world.mAllNPCs.find(npc->getUID()) == world.mAllNPCs.end());

            Script* task = nullptr;
            if (world.queryTask(&task, npc->getUID()))
                npc->linkTask(dynamic_cast<NpcTask*>(task));

            GameMap* map = mgr.getMap(npc->getMapId());
            if (map != nullptr)
            {
                world.mAllNPCs[npc->getUID()] = npc;
                map->enterRoom(*npc);

                npc = nullptr;
            }
            SAFE_DELETE(npc);
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
Database :: loadAllTasks()
{
    err_t err = ERROR_SUCCESS;

    World& world = World::getInstance();
    QDirIterator it("./tasks", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();

        QFileInfo info = it.fileInfo();
        if (info.isFile() && info.suffix() == "lua")
        {
            QString name = info.baseName();
            unsigned long int uid = strtoul(qPrintable(name), nullptr, 10);

            if (uid > 0 && uid < UINT32_MAX)
            {
                Script* task = nullptr;

                if (Entity::isNpc(uid))
                {
                    task = new NpcTask((uint32_t)uid, qPrintable(info.absoluteFilePath()));

                    ASSERT(task != nullptr);
                    ASSERT(world.mAllTasks.find(task->getUID()) == world.mAllTasks.end());
                }
                else
                {
                    task = new ItemTask((uint32_t)uid, qPrintable(info.absoluteFilePath()));

                    ASSERT(task != nullptr);
                    ASSERT(world.mAllTasks.find(task->getUID()) == world.mAllTasks.end());
                }

                if (task != nullptr)
                {
                    world.mAllTasks[task->getUID()] = task;
                    task = nullptr;
                }
                SAFE_DELETE(task);
            }
            else
            {
                LOG(WARN, "Could not convert the name of '%s' to a valid UID.",
                    qPrintable(info.absoluteFilePath()));
            }
        }
    }

    if (IS_SUCCESS(err))
    {
        fprintf(stdout, "Loaded all tasks.\n");
        LOG(INFO, "Loaded all tasks.");
    }

    return err;
}

err_t
Database :: loadAllMonsters()
{
    static const char cmd[] = "SELECT * FROM `monstertype`";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        while (ERROR_SUCCESS == err && query.next())
        {
            Monster::Info* info = new Monster::Info();
            ASSERT(info != nullptr);

            info->Id = (uint32_t)query.value(Monster::SQLDATA_ID).toUInt();
            info->Name = query.value(Monster::SQLDATA_NAME).toString().toStdString();
            info->Type = (uint8_t)query.value(Monster::SQLDATA_TYPE).toUInt();
            info->AIType = (uint8_t)query.value(Monster::SQLDATA_AI_TYPE).toUInt();
            info->Look = (uint32_t)query.value(Monster::SQLDATA_LOOK).toUInt();
            info->Level = (uint8_t)query.value(Monster::SQLDATA_LEVEL).toUInt();
            info->Life = (uint16_t)query.value(Monster::SQLDATA_LIFE).toUInt();
            info->EscapeLife = (uint16_t)query.value(Monster::SQLDATA_ESCAPE_LIFE).toUInt();
            info->AtkUser = (uint8_t)query.value(Monster::SQLDATA_ATK_USER).toUInt();
            info->MinAtk = (uint32_t)query.value(Monster::SQLDATA_MIN_ATK).toUInt();
            info->MaxAtk = (uint32_t)query.value(Monster::SQLDATA_MAX_ATK).toUInt();
            info->Defense = (uint32_t)query.value(Monster::SQLDATA_DEFENSE).toUInt();
            info->Dexterity = (uint8_t)query.value(Monster::SQLDATA_DEXTERITY).toUInt();
            info->Dodge = (uint8_t)query.value(Monster::SQLDATA_DODGE).toUInt();
            info->MagicType = (uint32_t)query.value(Monster::SQLDATA_MAGIC_TYPE).toUInt();
            info->MDef = (uint32_t)query.value(Monster::SQLDATA_MAGIC_DEFENSE).toUInt();
            info->HitRate = (uint32_t)query.value(Monster::SQLDATA_MAGIC_HIT_RATE).toUInt();
            info->ViewRange = (uint8_t)query.value(Monster::SQLDATA_VIEW_RANGE).toUInt();
            info->AtkRange = (uint8_t)query.value(Monster::SQLDATA_ATK_RANGE).toUInt();
            info->AtkSpeed = (uint16_t)query.value(Monster::SQLDATA_ATK_SPEED).toUInt();
            info->MoveSpeed = (uint16_t)query.value(Monster::SQLDATA_MOVE_SPEED).toUInt();
            info->RunSpeed = (uint16_t)query.value(Monster::SQLDATA_RUN_SPEED).toUInt();
            info->DropArmet = (uint8_t)query.value(Monster::SQLDATA_DROP_ARMET).toUInt();
            info->DropNecklace = (uint8_t)query.value(Monster::SQLDATA_DROP_NECKLACE).toUInt();
            info->DropArmor = (uint8_t)query.value(Monster::SQLDATA_DROP_ARMOR).toUInt();
            info->DropRing = (uint8_t)query.value(Monster::SQLDATA_DROP_RING).toUInt();
            info->DropWeapon = (uint8_t)query.value(Monster::SQLDATA_DROP_WEAPON).toUInt();
            info->DropShield = (uint8_t)query.value(Monster::SQLDATA_DROP_SHIELD).toUInt();
            info->DropShoes = (uint8_t)query.value(Monster::SQLDATA_DROP_SHOES).toUInt();
            info->DropMoney = (uint32_t)query.value(Monster::SQLDATA_DROP_MONEY).toUInt();
            info->DropHP = (uint32_t)query.value(Monster::SQLDATA_DROP_HP).toUInt();
            info->DropMP = (uint32_t)query.value(Monster::SQLDATA_DROP_MP).toUInt();
            info->ExtraBattleLvl = (uint16_t)query.value(Monster::SQLDATA_EXTRA_BATTLE_LVL).toUInt();
            info->ExtraExp = (uint16_t)query.value(Monster::SQLDATA_EXTRA_EXP).toUInt();
            info->ExtraDmg = (uint16_t)query.value(Monster::SQLDATA_EXTRA_DMG).toUInt();

            ASSERT(info != nullptr);
            ASSERT(mAllMonsters.find(info->Id) == mAllMonsters.end());

            mAllMonsters[info->Id] = info;
            info = nullptr;

            SAFE_DELETE(info);
        }

        if (IS_SUCCESS(err))
        {
            fprintf(stdout, "Loaded all monsters.\n");
            LOG(INFO, "Loaded all monsters.");
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
Database :: loadAllGenerators()
{
    static const char cmd[] = "SELECT * FROM `generator`";

    err_t err = ERROR_SUCCESS;

    QSqlQuery query(mConnection);
    query.prepare(cmd);

    LOG(DBG, "Executing SQL: %s", qPrintable(getSqlCommand(query)));

    if (query.exec())
    {
        World& world = World::getInstance();
        while (ERROR_SUCCESS == err && query.next())
        {
            Generator* generator = nullptr;
            err = Generator::makeGenerator(&generator, query);

            if (IS_SUCCESS(err))
            {
                world.mGeneratorMutex.lock();

                world.mAllGenerators.push_back(generator);
                generator = nullptr;

                world.mGeneratorMutex.unlock();
            }
            else
            {
                LOG(WARN, "Failed to create the generator %u. Ignoring error.",
                    (uint32_t)query.value(Generator::SQLDATA_ID).toInt());
                err = ERROR_SUCCESS;
            }

            SAFE_DELETE(generator);
        }
        world.startMonstersRegeneration();

        if (IS_SUCCESS(err))
        {
            fprintf(stdout, "Loaded all generators.\n");
            LOG(INFO, "Loaded all generators.");
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
Database :: getMonsterInfo(const Monster::Info** aOutInfo, uint32_t aId) const
{
    ASSERT_ERR(aOutInfo != nullptr && *aOutInfo == nullptr, ERROR_INVALID_POINTER);

    err_t err = ERROR_SUCCESS;

    map<uint32_t, Monster::Info*>::const_iterator it = mAllMonsters.find(aId);
    if (it != mAllMonsters.end())
    {
        *aOutInfo = it->second;
    }
    else
    {
        LOG(WARN, "Could not find the info of the monster %u.", aId);
        err = ERROR_NOT_FOUND;
    }

    return err;
}
