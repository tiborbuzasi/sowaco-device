/**
 * Class for Water Level Sensor
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "WaterLevelSensor.hpp"

#include <Arduino.h>

WaterLevelSensor::WaterLevelSensor(BLEService* service, const char* uuid, const char* name, DataType type,
        UltrasonicDistanceSensor* sensor, uint32_t permissions, uint32_t properties)
        : Sensor(service, uuid, name, type, permissions, properties), sensor(sensor) {}

void WaterLevelSensor::updateValue()
{
        unsigned int distance = sensor->getValue();

        // Find the nearest value, that is smaller than or equal to the measured delay
        unsigned char index = 0;
        while (distance < distances[index] && index < 20) ++index;

        // No smaller value found - the water level is at the highest
        if (index == 20)
        {
                value = 100;
        }

        // No bigger value found - the water is at the lowest
        else if (index == 0)
        {
                value = 0;
        }

        // Interpolation
        else
        {
                // Distance from smaller value is divided by the fifth of the distance between the two steps
                // This is the percent we are below the percent represented by the bigger value
                unsigned char offset = (distance - distances[index]) / ((distances[index - 1] - distances[index]) / 5);

                value = (index) * 5 - offset;
        }
        



#ifdef DEBUG
        Serial.print("Percent (%): ");
        Serial.println(value);
#endif
}