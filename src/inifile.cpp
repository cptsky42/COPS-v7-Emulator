/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#include "inifile.h"
#include <QSettings>
#include <QString>
#include <QStringList>

using namespace std;

IniFile :: IniFile()
    : mFile(nullptr)
{

}

IniFile :: ~IniFile()
{
    close();
}

err_t
IniFile :: open(const char* aPath)
{
    ASSERT_ERR(aPath != nullptr && aPath[0] != '\0', ERROR_INVALID_PARAMETER);

    err_t err = ERROR_SUCCESS;

    // delete any previous object
    SAFE_DELETE(mFile);

    mFile = new QSettings(aPath, QSettings::IniFormat);

    return err;
}

void
IniFile :: close()
{
    SAFE_DELETE(mFile);
}

void
IniFile :: getSections(vector<string>& aOutSections) const
{
    ASSERT(aOutSections.empty());

    QStringList sections = mFile->childGroups();
    for (QStringList::const_iterator
            it = sections.begin(), end = sections.end();
         it != end; ++it)
    {
        const QString& section = *it;
        aOutSections.push_back(section.toStdString());
    }
}

int8_t
IniFile :: readInt8(const char* aKey, int8_t aDefault)
{
    err_t err = ERROR_SUCCESS;

    int value;
    err = readInteger(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return (int8_t)value;
}

uint8_t
IniFile :: readUInt8(const char* aKey, uint8_t aDefault)
{
    err_t err = ERROR_SUCCESS;

    unsigned int value;
    err = readUInteger(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return (uint8_t)value;
}

int16_t
IniFile :: readInt16(const char* aKey, int16_t aDefault)
{
    err_t err = ERROR_SUCCESS;

    int value;
    err = readInteger(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return (int16_t)value;
}

uint16_t
IniFile :: readUInt16(const char* aKey, uint16_t aDefault)
{
    err_t err = ERROR_SUCCESS;

    unsigned int value;
    err = readUInteger(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return (uint16_t)value;
}

int32_t
IniFile :: readInt32(const char* aKey, int32_t aDefault)
{
    err_t err = ERROR_SUCCESS;

    int value;
    err = readInteger(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return (int32_t)value;
}

uint32_t
IniFile :: readUInt32(const char* aKey, uint32_t aDefault)
{
    err_t err = ERROR_SUCCESS;

    unsigned int value;
    err = readUInteger(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return (uint32_t)value;
}

float
IniFile :: readFloat(const char* aKey, float aDefault)
{
    err_t err = ERROR_SUCCESS;

    double value;
    err = readDouble(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return (float)value;
}

double
IniFile :: readDouble(const char* aKey, double aDefault)
{
    err_t err = ERROR_SUCCESS;

    double value;
    err = readDouble(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return (double)value;
}

bool
IniFile :: readBoolean(const char* aKey, bool aDefault)
{
    err_t err = ERROR_SUCCESS;

    bool value;
    err = readBoolean(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return value;
}

string
IniFile :: readString(const char* aKey, const char* aDefault)
{
    err_t err = ERROR_SUCCESS;

    string value;
    err = readString(value, aKey);

    if (!IS_SUCCESS(err))
    {
        value = aDefault;
    }

    return value;
}

err_t
IniFile :: readInteger(int& aOutValue, const char* aKey)
{
    ASSERT_ERR(aKey != nullptr && aKey[0] != '\0', ERROR_INVALID_PARAMETER);
    ASSERT_ERR(&aOutValue != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    if (mFile != nullptr)
    {
        aOutValue = mFile->value(aKey, 0).toInt();
    }
    else
    {
        err = ERROR_READ_FAULT;
    }

    return err;
}

err_t
IniFile :: readUInteger(unsigned int& aOutValue, const char* aKey)
{
    ASSERT_ERR(aKey != nullptr && aKey[0] != '\0', ERROR_INVALID_PARAMETER);
    ASSERT_ERR(&aOutValue != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    if (mFile != nullptr)
    {
        aOutValue = mFile->value(aKey, 0).toUInt();
    }
    else
    {
        err = ERROR_READ_FAULT;
    }

    return err;
}

err_t
IniFile :: readDouble(double& aOutValue, const char* aKey)
{
    ASSERT_ERR(aKey != nullptr && aKey[0] != '\0', ERROR_INVALID_PARAMETER);
    ASSERT_ERR(&aOutValue != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    if (mFile != nullptr)
    {
        aOutValue = mFile->value(aKey, 0.0).toDouble();
    }
    else
    {
        err = ERROR_READ_FAULT;
    }

    return err;
}

err_t
IniFile :: readBoolean(bool& aOutValue, const char* aKey)
{
    ASSERT_ERR(aKey != nullptr && aKey[0] != '\0', ERROR_INVALID_PARAMETER);
    ASSERT_ERR(&aOutValue != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    if (mFile != nullptr)
    {
        aOutValue = mFile->value(aKey, false).toBool();
    }
    else
    {
        err = ERROR_READ_FAULT;
    }

    return err;
}

err_t
IniFile :: readString(string& aOutValue, const char* aKey)
{
    ASSERT_ERR(aKey != nullptr && aKey[0] != '\0', ERROR_INVALID_PARAMETER);
    ASSERT_ERR(&aOutValue != nullptr, ERROR_INVALID_REFERENCE);

    err_t err = ERROR_SUCCESS;

    if (mFile != nullptr)
    {
        aOutValue = mFile->value(aKey, "").toString().toStdString();
    }
    else
    {
        err = ERROR_READ_FAULT;
    }

    return err;
}
