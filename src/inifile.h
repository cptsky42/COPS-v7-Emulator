/*
 * ****** COPS v7 Emulator - Open Source ******
 * Copyright (C) 2012 - 2014 Jean-Philippe Boivin
 *
 * Please read the WARNING, DISCLAIMER and PATENTS
 * sections in the LICENSE file.
 */

#ifndef _COPS_V7_EMULATOR_INI_FILE_H_
#define _COPS_V7_EMULATOR_INI_FILE_H_

#include "common.h"
#include <string>
#include <vector>

#if UINT32_MAX < UINT_MAX
#error "The IniFile class requires that a int be of 32-bit."
#endif

class QSettings;

/**
 * A INI file parser.
 */
class IniFile
{
public:
    /* constructor */
    IniFile();

    /* destructor */
    ~IniFile();

    /**
     * Open an INI file for reading.
     *
     * @param[in]   aPath   the path of the INI file
     *
     * @retval ERROR_SUCCESS on success
     * @returns Error code otherwise
     */
    err_t open(const char* aPath);
    err_t open(const std::string& aPath)
    { return open(aPath.c_str()); }

    /**
     * Close the stream. It will be called automatically.
     */
    void close();

    /**
     * Read a signed 8-bit integer.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is 0.
     *
     * @retval The read value
     */
    int8_t readInt8(const char* aKey, int8_t aDefault = 0);
    int8_t readInt8(const std::string& aKey, int8_t aDefault = 0)
    { return readInt8(aKey.c_str(), aDefault); }

    /**
     * Read a unsigned 8-bit integer.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is 0.
     *
     * @retval The read value
     */
    uint8_t readUInt8(const char* aKey, uint8_t aDefault = 0);
    uint8_t readUInt8(const std::string& aKey, uint8_t aDefault = 0)
    { return readUInt8(aKey.c_str(), aDefault); }

    /**
     * Read a signed 16-bit integer.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is 0.
     *
     * @retval The read value
     */
    int16_t readInt16(const char* aKey, int16_t aDefault = 0);
    int16_t readInt16(const std::string& aKey, int16_t aDefault = 0)
    { return readInt16(aKey.c_str(), aDefault); }

    /**
     * Read a unsigned 16-bit integer.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is 0.
     *
     * @retval The read value
     */
    uint16_t readUInt16(const char* aKey, uint16_t aDefault = 0);
    uint16_t readUInt16(const std::string& aKey, uint16_t aDefault = 0)
    { return readUInt16(aKey.c_str(), aDefault); }

    /**
     * Read a signed 32-bit integer.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is 0.
     *
     * @retval The read value
     */
    int32_t readInt32(const char* aKey, int32_t aDefault = 0);
    int32_t readInt32(const std::string& aKey, int32_t aDefault = 0)
    { return readInt32(aKey.c_str(), aDefault); }

    /**
     * Read a unsigned 32-bit integer.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is 0.
     *
     * @retval The read value
     */
    uint32_t readUInt32(const char* aKey, uint32_t aDefault = 0);
    uint32_t readUInt32(const std::string& aKey, uint32_t aDefault = 0)
    { return readUInt32(aKey.c_str(), aDefault); }

    /**
     * Read a float.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is 0.
     *
     * @retval The read value
     */
    float readFloat(const char* aKey, float aDefault = 0.0);
    float readFloat(const std::string& aKey, float aDefault = 0.0)
    { return readFloat(aKey.c_str(), aDefault); }

    /**
     * Read a double.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is 0.
     *
     * @retval The read value
     */
    double readDouble(const char* aKey, double aDefault = 0.0);
    double readDouble(const std::string& aKey, double aDefault = 0.0)
    { return readDouble(aKey.c_str(), aDefault); }

    /**
     * Read a boolean.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is false.
     *
     * @retval The read value
     */
    bool readBoolean(const char* aKey, bool aDefault = false);
    bool readBoolean(const std::string& aKey, bool aDefault = false)
    { return readBoolean(aKey.c_str(), aDefault); }

    /**
     * Read a string.
     *
     * @param[in]  aKey       the key of the INI file (SECTION/KEY)
     * @param[in]  aDefault   (optional) the default value
     *                        By default, the value is an empty string.
     *
     * @retval The read value
     */
    std::string readString(const char* aKey, const char* aDefault = "");
    std::string readString(const std::string& aKey, const std::string& aDefault = "")
    { return readString(aKey.c_str(), aDefault.c_str()); }

public:
    /** Determine whether or not a file is open. */
    bool isOpen() const { return mFile != nullptr; }

    /** Get all the sections. */
    void getSections(std::vector<std::string>& aOutSections) const;

private:
    /** Read an integer. */
    err_t readInteger(int& aOutValue, const char* aKey);
    /** Read an unsigned integer. */
    err_t readUInteger(unsigned int& aOutValue, const char* aKey);
    /** Read a double. */
    err_t readDouble(double& aOutValue, const char* aKey);
    /** Read a boolean. */
    err_t readBoolean(bool& aOutValue, const char* aKey);
    /** Read a string. */
    err_t readString(std::string& aOutValue, const char* aKey);

private:
    QSettings* mFile; //!< Qt settings file
};

#endif // _COPS_V7_EMULATOR_INI_FILE_H_
