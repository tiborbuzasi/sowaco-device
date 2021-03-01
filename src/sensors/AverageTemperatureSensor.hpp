/**
 * Class for Average Temperature Sensor - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_AVERAGE_TEMPERATURE_SENSOR_H
#define SOWACO_AVERAGE_TEMPERATURE_SENSOR_H

#include "TemperatureSensor.hpp"

class AverageTemperatureSensor : public Sensor
{
public:
    /**
     * Create a new temperature sensor object
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param sensors Array of temperature sensors
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    AverageTemperatureSensor(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        TemperatureSensor* sensors[3],
        uint32_t permissions = BT_PERMISSION_READ,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Update sensor value
     */
    void updateValue();

private:
    /**
     * Array of temperature sensors
     */
    TemperatureSensor* sensors[3] = {};
};

#endif // SOWACO_AVERAGE_TEMPERATURE_SENSOR_H