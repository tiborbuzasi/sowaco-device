/**
 * Class for Temperature Sensor
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "TemperatureSensor.hpp"

TemperatureSensor::TemperatureSensor(BLEService* service, const char* uuid, const char* name, DataType type,
    DallasTemperature* sensorPack, const DeviceAddress address, uint32_t permissions, uint32_t properties)
    : Sensor(service, uuid, name, type, permissions, properties), sensorPack(sensorPack)
{
    for (int i = 0; i < 8; ++i)
    {
        deviceAddress[i] = address[i];
    }
}

void TemperatureSensor::updateValue()
{
    sensorPack->requestTemperaturesByAddress(deviceAddress);

    delay(100);

    float tempValue = sensorPack->getTempC(deviceAddress);

    value = tempValue * 100;

#ifdef DEBUG
        Serial.print("Temperature (C): ");
        Serial.println(value);
#endif

}