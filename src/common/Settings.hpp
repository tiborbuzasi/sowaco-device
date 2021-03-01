/**
 * Class for Settings - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_SETTINGS_H
#define SOWACO_SETTINGS_H

#include <Preferences.h>

class Settings
{
public:
    /**
     * Read string based setting value
     *
     * @param group Name of the settings group
     * @param key Name of the settings key
     * @param defaultValue Default value of the setting
     * @return Value of the setting
     */
    static const char* readString(const char* group, const char* key, const char* defaultValue);

    /**
     * Read 8 bit unsigned integer based setting value
     * 
     * @param group Name of the settings group
     * @param key Name of the settings key
     * @param defaultValue Default value of the setting
     * @return Value of the setting
     */
    static const uint8_t readUint8(const char* group, const char* key, const uint8_t defaultValue);

    /**
     * Read 32 bit unsigned integer based setting value
     *
     * @param group Name of the settings group
     * @param key Name of the settings key
     * @param defaultValue Default value of the setting
     * @return Value of the setting
     */
    static const uint32_t readUint32(const char* group, const char* key, const uint32_t defaultValue);

    /**
     * Write string based setting value
     *
     * @param group Name of the settings group
     * @param key Name of the settings key
     * @param value Value to store
     */
    static bool writeString(const char* group, const char* key, const char* value);

    /**
     * Write 8 bit unsigned integer based setting value
     *
     * @param group Name of the settings group
     * @param key Name of the settings key
     * @param value Value to store
     */
    static bool writeUint8(const char* group, const char* key, const uint8_t value);

    /**
     * Write 32 bit unsigned integer based setting value
     *
     * @param group Name of the settings group
     * @param key Name of the settings key
     * @param value Value to store
     */
    static bool writeUint32(const char* group, const char* key, const uint32_t value);

    /**
     * Reset process settings
     */
    static bool resetProcess();

    /**
     * Reset to factory settings
     */
    static bool resetToFactory();

private:
    /**
     * Debug log to console
     *
     * @param group Name of the settings group
     * @param key Name of the settings key
     * @param success Successful saving
     */
    static void log(const char* group, const char* key, bool success);
};

#endif // SOWACO_SETTINGS_H