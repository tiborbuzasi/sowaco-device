/**
 * Class for Ultrasonic Distance Sensor - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_ULTRASONIC_DISTANCE_SENSOR_H
#define SOWACO_ULTRASONIC_DISTANCE_SENSOR_H

#include "TemperatureSensor.hpp"
#include "UltrasonicSensor.hpp"

class UltrasonicDistanceSensor : public Sensor
{
public:
    /**
     * Create a new ultrasonic distance sensor object
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param sensor Pointer to ultrasonic sensor object
     * @param temperatureSensor Pointer to temperature sensor object
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    UltrasonicDistanceSensor(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        UltrasonicSensor* sensor,
        TemperatureSensor* temperatureSensor,
        uint32_t permissions = BT_PERMISSION_READ,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Update sensor value
     */
    void updateValue();

private:
    /**
     * Ultrasonic sensor object
     */
    UltrasonicSensor* sensor;

    /**
     * Temperature sensor object
     */
    TemperatureSensor* temperatureSensor;
};

#endif // SOWACO_ULTRASONIC_DISTANCE_SENSOR_H