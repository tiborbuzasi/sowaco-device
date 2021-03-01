/**
 * Class for Setting
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "Setting.hpp"

#include "../bluetooth/BtUuid.hpp"
#include "../common/Settings.hpp"

Setting::Setting(BLEService* service, const char* uuid, const char* name, DataType type, const char* group,
    const char* key, bool storable, uint32_t permissions, uint32_t properties)
    : BtCharacteristic(service, uuid, name, type, permissions, properties),
    group(group), key(key), type(type), storable(storable)
{
    characteristic->setCallbacks(this);
}

unsigned char Setting::getValue() { return 0; }
unsigned int Setting::getIntValue() { return 0; }
const char* Setting::getStringValue() { return " "; }

void Setting::setValue(unsigned char value) {}
void Setting::setValue(unsigned int value) {}
void Setting::setValue(const char* value) {}
