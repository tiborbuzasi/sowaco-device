/**
 * Class for Ultrasonic Sensor
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "UltrasonicSensor.hpp"

#include <DallasTemperature.h>
#include <OneWire.h>

UltrasonicSensor::UltrasonicSensor(BLEService* service, const char* uuid, const char* name, DataType type,
    unsigned char triggerPin, unsigned char echoPin, uint32_t permissions, uint32_t properties)
    : Sensor(service, uuid, name, type, permissions, properties), triggerPin(triggerPin), echoPin(echoPin) {}

void UltrasonicSensor::updateValue()
{
    unsigned int sum = 0;
    unsigned int values[5];
    unsigned int min = timeout;
    unsigned int max = 0;

    // Run measurement 5 times and prepare calculation
    for (unsigned char i = 0; i < 5; ++i)
    {
        // Send trigger signal
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(5);
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(20);
        digitalWrite(triggerPin, LOW);

        // Listen for response
        values[i] = pulseIn(echoPin, HIGH, timeout);
        sum += values[i];

        // Check for minimum
        if (values[i] < min) {
            min = values[i];
        }

        // Check for maximum
        if (values[i] > max) {
            max = values[i];
        }

#ifdef DEBUG
        Serial.print("Delay (ms) #");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(values[i]);
#endif

    }

    // Calculate average value by removing the minimum and maximum values
    value = (sum - min - max) / 3;

#ifdef DEBUG
    Serial.print("Delay (ms): ");
    Serial.println(value);
#endif
}