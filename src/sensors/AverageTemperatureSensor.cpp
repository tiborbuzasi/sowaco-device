/**
 * Class for Average Temperature Sensor
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "AverageTemperatureSensor.hpp"

AverageTemperatureSensor::AverageTemperatureSensor(BLEService* service, const char* uuid, const char* name, DataType type,
    TemperatureSensor* sensors[3], uint32_t permissions, uint32_t properties)
    : Sensor(service, uuid, name, type, permissions, properties)
{
    for (int i = 0; i < 3; ++i)
    {
        this->sensors[i] = sensors[i];
    }
}

void AverageTemperatureSensor::updateValue()
{
    unsigned int sum = 0;
    int divider = 0;

    // Sum the value of the sensors
    for (int i = 0; i < 3; ++i)
    {
        if (sensors[i])
        {
            sum += sensors[i]->getValue();
            divider++;
            
            // Prominent sensor has the weight of 2
            if (i == 0)
            {
                sum *= 2;
                divider++;
            }
        }
    }

    // Get average by dividing the sum
    if (divider != 0)
    {
        value = sum / divider;
    }
    // No sensor is present
    else
    {
        value = -1;
    }
    


#ifdef DEBUG
        Serial.print("Average Temperature (C): ");
        Serial.println(value);
#endif

}