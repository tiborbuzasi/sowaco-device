/**
 * Class for Actuator
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include <Arduino.h>
#include "Actuator.hpp"

Actuator::Actuator(BLEService* service, const char* uuid, const char* name, DataType type,
    unsigned char pin, uint32_t permissions, uint32_t properties)
    : BtCharacteristic(service, uuid, name, type, permissions, properties), pin(pin)
{
    value = false;
    characteristic->setValue((uint8_t*)&value, 1);

    notificationNeeded = true;
}

bool Actuator::getValue()
{
    return value;
}

void Actuator::setValue(bool value)
{
    this->value = value;
    characteristic->setValue((uint8_t*)&value, 1);
    digitalWrite(pin, value ? HIGH : LOW);

    notificationNeeded = true;
}