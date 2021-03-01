/**
 * Class for Ultrasonic Distance Sensor
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "UltrasonicDistanceSensor.hpp"

#include <Arduino.h>

UltrasonicDistanceSensor::UltrasonicDistanceSensor(BLEService* service, const char* uuid, const char* name, DataType type,
    UltrasonicSensor* sensor, TemperatureSensor* temperatureSensor, uint32_t permissions, uint32_t properties)
    : Sensor(service, uuid, name, type, permissions, properties), sensor(sensor), temperatureSensor(temperatureSensor) {}

void UltrasonicDistanceSensor::updateValue()
{
        // Temperature dependence correction for sound speed
        double soundSpeed = 331.4 * sqrt((double) temperatureSensor->getValue() / 27300 + 1);

        value = sensor->getValue() * soundSpeed / 2 / 1000;

#ifdef DEBUG
        Serial.print("Distance (mm): ");
        Serial.println(value);
#endif
}