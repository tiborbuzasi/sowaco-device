/**
 * Class for Process Setting
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "ProcessSetting.hpp"

ProcessSetting::ProcessSetting(BLEService* service, const char* uuid, const char* name, DataType type,
    const char* group, const char* key, uint8_t defaultValue, uint32_t permissions, uint32_t properties)
    : Int8Setting(service, uuid, name, type, group, key, defaultValue, true, permissions, properties)
{
    newValue = value;
}

unsigned char ProcessSetting::getNewValue()
{
    return newValue;
}

void ProcessSetting::onWrite(BLECharacteristic* characteristic)
{
    newValue = *characteristic->getData();
}