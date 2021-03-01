/**
 * Temperature Sensors Serial Codes - Header File
 * 
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_TEMPERATURE_SENSORS_SERIAL_CODES_H
#define SOWACO_TEMPERATURE_SENSORS_SERIAL_CODES_H

#include <Arduino.h>

const uint8_t sensor1SerialCode[8] = {0x28, 0xC1, 0xC9, 0x56, 0xB5, 0x01, 0x3C, 0xE6};
const uint8_t sensor2SerialCode[8] = {0x28, 0x54, 0x28, 0x56, 0xB5, 0x01, 0x3C, 0xAB};
const uint8_t sensor3SerialCode[8] = {0x28, 0xC7, 0xBD, 0x56, 0xB5, 0x01, 0x3C, 0x26};
const uint8_t sensor4SerialCode[8] = {0x28, 0x24, 0x21, 0x56, 0xB5, 0x01, 0x3C, 0xC0};

#endif // SOWACO_TEMPERATURE_SENSORS_SERIAL_CODES_H