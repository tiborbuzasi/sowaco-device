/**
 * Class for String Setting
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "StringSetting.hpp"

#include "../common/Settings.hpp"

StringSetting::StringSetting(BLEService* service, const char* uuid, const char* name, DataType type, const char* group, const char* key, const char* defaultValue, uint32_t permissions, uint32_t properties)
    : Setting(service, uuid, name, type, group, key, true, permissions, properties)
{
    value = (char*)Settings::readString(group, key, defaultValue);
    characteristic->setValue(value);

    notificationNeeded = true;
}

const char* StringSetting::getStringValue()
{
    return value;
}

void StringSetting::setValue(const char* value)
{
    this->value = value;
    Settings::writeString(group, key, value);
    characteristic->setValue(value);
    
    notificationNeeded = true;
}

void StringSetting::onWrite(BLECharacteristic* characteristic)
{
    value = (const char*)characteristic->getValue().c_str();
    Settings::writeString(group, key, value);
}