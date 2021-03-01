/**
 * Class for Temperature Sensor - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_TEMPERATURE_SENSOR_H
#define SOWACO_TEMPERATURE_SENSOR_H

#include <DallasTemperature.h>
#include <OneWire.h>

#include "Sensor.hpp"

class TemperatureSensor : public Sensor
{
public:
    /**
     * Create a new temperature sensor object
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param sensorPack Pointer to the DallasTemperature object
     * @param address Device address
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    TemperatureSensor(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        DallasTemperature* sensorPack,
        const DeviceAddress address,
        uint32_t permissions = BT_PERMISSION_READ,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Update sensor value
     */
    void updateValue();

private:
    /**
     * Sensor device ID
     */
    DeviceAddress deviceAddress;

    /**
     * DallasTemperature object
     */
    DallasTemperature* sensorPack;
};

#endif // SOWACO_TEMPERATURE_SENSOR_H