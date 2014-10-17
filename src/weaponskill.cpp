/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "weaponskill.h"
#include "world.h"
#include <QSqlQuery>

/* static */
err_t
WeaponSkill :: createSkill(WeaponSkill** aOutSkill, const QSqlQuery& aQuery)
{
    ASSERT_ERR(aOutSkill != nullptr && *aOutSkill == nullptr, ERROR_INVALID_POINTER);
    ASSERT_ERR(&aQuery != nullptr, ERROR_INVALID_REFERENCE);

    static const World& world = World::getInstance(); // singleton

    err_t err = ERROR_SUCCESS;

    uint32_t id = (uint32_t)aQuery.value(SQLDATA_ID).toUInt();
    uint16_t type = (uint16_t)aQuery.value(SQLDATA_TYPE).toUInt();

    Player* player = nullptr;
    if (!world.queryPlayer(&player, (uint32_t)aQuery.value(SQLDATA_OWNER_ID).toUInt()))
        err = ERROR_NOT_FOUND;

    if (IS_SUCCESS(err))
    {
        WeaponSkill* skill = new WeaponSkill(id, type, *player);

        skill->mLevel = (uint8_t)aQuery.value(SQLDATA_LEVEL).toUInt();
        skill->mExp = (uint32_t)aQuery.value(SQLDATA_EXP).toUInt();
        skill->mOldLevel = (uint8_t)aQuery.value(SQLDATA_OLD_LEVEL).toUInt();
        skill->mIsUnlearn = aQuery.value(SQLDATA_UNLEARN).toBool();

        if (IS_SUCCESS(err))
        {
            *aOutSkill = skill;
            skill = nullptr;
        }
        SAFE_DELETE(skill);
    }

    return err;
}
