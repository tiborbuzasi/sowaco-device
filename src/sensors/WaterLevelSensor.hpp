/**
 * Class for Water Level Sensor - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_WATER_LEVEL_SENSOR_H
#define SOWACO_WATER_LEVEL_SENSOR_H

#include "UltrasonicDistanceSensor.hpp"

class WaterLevelSensor : public Sensor
{
public:
    /**
     * Create a new water level sensor object
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param sensor Pointer to ultrasonic distance sensor object
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    WaterLevelSensor(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        UltrasonicDistanceSensor* sensor,
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
    UltrasonicDistanceSensor* sensor;

    /**
     * Array of ultrasonic sensor distances
     * 
     * Required for water level calculation
     * Starts from 0%, ends with 100% with a 5% step
     */
    unsigned short int distances[21] =   { 805, 777, 735, 718, 688, 652,
                                                615, 589, 568, 544, 523,
                                                495, 459, 428, 408, 380,
                                                358, 320, 282, 255, 215 };
};

#endif // SOWACO_WATER_LEVEL_SENSOR_H