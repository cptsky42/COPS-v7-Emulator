/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "magic.h"
#include "database.h"
#include "world.h"
#include <QSqlQuery>

/* static */
err_t
Magic :: createMagic(Magic** aOutMagic, const QSqlQuery& aQuery)
{
    ASSERT_ERR(aOutMagic != nullptr && *aOutMagic == nullptr, ERROR_INVALID_POINTER);
    ASSERT_ERR(&aQuery != nullptr, ERROR_INVALID_REFERENCE);

    static const Database& db = Database::getInstance(); // singleton
    static const World& world = World::getInstance(); // singleton

    err_t err = ERROR_SUCCESS;

    uint32_t id = (uint32_t)aQuery.value(SQLDATA_ID).toUInt();
    uint16_t type = (uint16_t)aQuery.value(SQLDATA_TYPE).toUInt();
    uint16_t level = (uint16_t)aQuery.value(SQLDATA_LEVEL).toUInt();

    Player* player = nullptr;
    if (!world.queryPlayer(&player, (uint32_t)aQuery.value(SQLDATA_OWNER_ID).toUInt()))
        err = ERROR_NOT_FOUND;

    const Magic::Info* info = nullptr;
    DOIF(err, db.getMagicInfo(&info, type, level));

    if (IS_SUCCESS(err))
    {
        Magic* magic = new Magic(id, *info, *player);

        magic->mExp = (uint32_t)aQuery.value(SQLDATA_EXP).toUInt();
        magic->mOldLevel = (uint8_t)aQuery.value(SQLDATA_OLD_LEVEL).toUInt();
        magic->mIsUnlearn = aQuery.value(SQLDATA_UNLEARN).toBool();

        if (IS_SUCCESS(err))
        {
            *aOutMagic = magic;
            magic = nullptr;
        }
        SAFE_DELETE(magic);
    }

    return err;
}
