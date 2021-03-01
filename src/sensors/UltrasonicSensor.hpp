/**
 * Class for Ultrasonic Sensor - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_ULTRASONIC_SENSOR_H
#define SOWACO_ULTRASONIC_SENSOR_H

#include "Sensor.hpp"

class UltrasonicSensor : public Sensor
{
public:
    /**
     * Create a new ultrasonic sensor object
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param triggerPin GPIO pin for trigger
     * @param echoPin GPIO pin for echo
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    UltrasonicSensor(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        unsigned char triggerPin,
        unsigned char echoPin,
        uint32_t permissions = BT_PERMISSION_READ,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Update sensor value
     */
    void updateValue();

private:
    /**
     * Measurement timeout (in microseconds)
     */
    const unsigned int timeout = 30000;

    /**
     * Trigger GPIO pin
     */
    unsigned char triggerPin;

    /**
     * Echo GPIO pin
     */
    unsigned char echoPin;
};

#endif // SOWACO_ULTRASONIC_SENSOR_H