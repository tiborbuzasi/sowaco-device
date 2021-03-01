/**
 * Class for Settings
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "Settings.hpp"

#include <Preferences.h>

#include "../app.hpp"

const char* Settings::readString(const char* group, const char* key, const char* defaultValue)
{
    Preferences preferences;
    char value[30] = "";
    char* pValue = value;
    size_t length;

    preferences.begin(group, true);
    length = preferences.getString(key, pValue, 30);
    preferences.end();

    if (!length)
    {
        return defaultValue;
    }

    return pValue;
}

const uint8_t Settings::readUint8(const char* group, const char* key, uint8_t defaultValue)
{
    Preferences preferences;
    uint8_t value = 0;

    preferences.begin(group, true);
    value = preferences.getUChar(key, defaultValue);
    preferences.end();

    return value;
}

const uint32_t Settings::readUint32(const char* group, const char* key, uint32_t defaultValue)
{
    Preferences preferences;
    uint32_t value = 0;

    preferences.begin(group, true);
    value = preferences.getUInt(key, defaultValue);
    preferences.end();

    return value;
}

bool Settings::writeString(const char* group, const char* key, const char* value)
{
    Preferences preferences;
    bool success = false;

    preferences.begin(group, false);
    success = preferences.putString(key, value);
    preferences.end();

    log(group, key, success);
    return success;
}

bool Settings::writeUint8(const char* group, const char* key, const uint8_t value)
{
    Preferences preferences;
    bool success = false;

    preferences.begin(group, false);
    success = preferences.putUChar(key, value);
    preferences.end();

    log(group, key, success);
    return success;
}

bool Settings::writeUint32(const char* group, const char* key, const uint32_t value)
{
    Preferences preferences;
    bool success = false;

    preferences.begin(group, false);
    success = preferences.putUInt(key, value);
    preferences.end();

    log(group, key, success);
    return success;
}

bool Settings::resetProcess() {
    bool success = true;

    if (!writeUint8("process", "operationMode", OPERATION_MODE)) success = false;
    if (!writeUint8("process", "opStartTime", OPERATION_START_TIME)) success = false;
    if (!writeUint8("process", "opEndTime", OPERATION_END_TIME)) success = false;
    if (!writeUint8("process", "goalLevel", GOAL_LEVEL)) success = false;
    if (!writeUint8("process", "goalTemperature", GOAL_TEMPERATURE)) success = false;
    if (!writeUint8("process", "goalTime", GOAL_TIME)) success = false;
    if (!writeUint8("process", "pipeDrTime", PIPE_DRAINING_TIME)) success = false;
    if (!writeUint8("process", "tankDrTime", TANK_DRAINING_TIME)) success = false;

    return success;
}

bool Settings::resetToFactory()
{
    bool success = true;

    if (!writeString("device", "name", DEVICE_NAME)) success = false;
    if (!writeUint32("device", "passKey", DEVICE_PASSKEY)) success = false;
    if (!resetProcess()) success = false;

    return success;
}

void Settings::log(const char* group, const char* key, bool success)
{

#ifdef DEBUG
    Serial.print("Setting ");
    Serial.print(group);
    Serial.print(".");
    Serial.print(key);
    if (success)
    {
        Serial.println(" was successful.");
    }
    else
    {
        Serial.println(" failed.");
    }
#endif

}