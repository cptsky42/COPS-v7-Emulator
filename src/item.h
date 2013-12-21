/**
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_ITEM_H_
#define _COPS_V7_EMULATOR_ITEM_H_

#include "common.h"
#include <string>

class Item
{
public:
    struct Info
    {
    public:
        uint32_t Id;
        std::string Name;
        uint8_t ReqProf;
        uint8_t ReqWeaponSkill;
        uint8_t ReqLevel;
        uint8_t ReqSex;
        uint16_t ReqForce;
        uint16_t ReqSpeed;
        uint16_t ReqHealth;
        uint16_t ReqSoul;
        uint8_t Monopoly;
        uint16_t Weight;
        uint32_t Price;
        uint32_t Task; //TODO: ItemTask*
        uint16_t MaxAtk;
        uint16_t MinAtk;
        int16_t Defense;
        int16_t Dexterity;
        int16_t Dodge;
        int16_t Life;
        int16_t Mana;
        uint16_t Amount;
        uint16_t AmountLimit;
        uint8_t Status;
        uint8_t Gem1;
        uint8_t Gem2;
        uint8_t Magic1;
        uint8_t Magic2;
        uint8_t Magic3;
        uint16_t MagicAtk;
        uint16_t MagicDef;
        uint16_t AtkRange;
        uint16_t AtkSpeed;
    };

public:
    Item(uint32_t aUID, Item::Info& aInfo);
    ~Item();

private:
    const uint32_t mUID;
    Item::Info& mInfo;
};

#endif // _COPS_V7_EMULATOR_ITEM_H_
