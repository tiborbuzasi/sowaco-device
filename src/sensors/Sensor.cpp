/**
 * Class for Sensor
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "Sensor.hpp"

#include <Arduino.h>

#include "../gpio.hpp"

Sensor::Sensor(BLEService* service, const char* uuid, const char* name, DataType type,
    uint32_t permissions, uint32_t properties)
    : BtCharacteristic(service, uuid, name, type, permissions, properties), type(type) {}

void Sensor::update()
{
    updateValue();
    characteristic->setValue((uint32_t&)value);
    
    notificationNeeded = true;
}

unsigned int Sensor::getValue()
{
    return value;
}