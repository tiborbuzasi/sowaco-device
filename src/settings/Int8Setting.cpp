/**
 * Class for 8-bit Integer Setting
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "Int8Setting.hpp"

#include "../common/Settings.hpp"

Int8Setting::Int8Setting(BLEService* service, const char* uuid, const char* name, DataType type,
    const char* group, const char* key, uint8_t defaultValue, bool storable, uint32_t permissions, uint32_t properties)
    : Setting(service, uuid, name, type, group, key, storable, permissions, properties)
{
    if (type == BOOLEAN)
    {
        defaultValue = (bool)defaultValue;
    }

    if (storable) {
        value = defaultValue;
    }
    else
    {
        value = Settings::readUint8(group, key, defaultValue);
    }
    characteristic->setValue(&value, 1);

    notificationNeeded = true;
}

unsigned char Int8Setting::getValue()
{
    return value;
}

void Int8Setting::setValue(unsigned char value)
{
    this->value = value;
    if (type == BOOLEAN)
    {
        value = (bool)value;
    }

    if (storable)
    {
        Settings::writeUint8(group, key, value);
    }
    characteristic->setValue(&value, 1);

    notificationNeeded = true;
}

void Int8Setting::onWrite(BLECharacteristic* characteristic)
{
    value = *characteristic->getData();
    if (type == BOOLEAN)
    {
        value = (bool)value;
    }

    if (storable)
    {
        Settings::writeUint8(group, key, (uint8_t)value);
    }
}