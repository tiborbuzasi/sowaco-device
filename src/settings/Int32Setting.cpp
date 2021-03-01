/**
 * Class for 32-bit Integer Setting
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "Int32Setting.hpp"

#include "../common/Settings.hpp"

Int32Setting::Int32Setting(BLEService* service, const char* uuid, const char* name, DataType type,
    const char* group, const char* key, uint32_t defaultValue, uint32_t permissions, uint32_t properties)
    : Setting(service, uuid, name, type, group, key, true, permissions, properties)
{
    value = Settings::readUint32(group, key, defaultValue);
    characteristic->setValue(value);

    notificationNeeded = true;
}

unsigned int Int32Setting::getIntValue()
{
    return value;
}

void Int32Setting::setValue(unsigned int value)
{
    this->value = value;
    Settings::writeUint32(group, key, value);
    characteristic->setValue(value);
    
    notificationNeeded = true;
}

void Int32Setting::onWrite(BLECharacteristic* characteristic)
{
    value = (unsigned int)characteristic->getData();
}