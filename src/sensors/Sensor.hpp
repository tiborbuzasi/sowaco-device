/**
 * Class for Sensor - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_SENSOR_H
#define SOWACO_SENSOR_H

#include "../bluetooth/BtCharacteristic.hpp"
#include "../app.hpp"

class Sensor : public BtCharacteristic
{
public:
    /**
     * Create a new sensor object
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    Sensor(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        uint32_t permissions = BT_PERMISSION_READ,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Update sensor value
     */
    void update();

    /**
     * Update sensor value (subclass implementation)
     */
    virtual void updateValue() = 0;

    /**
     * Get sensor value
     *
     * @return Measured value
     */
    unsigned int getValue();

protected:
    /**
     * Sensor type
     */
    DataType type;

    /**
     * Sensor value
     */
    unsigned int value = 0;
};

#endif // SOWACO_SENSOR_H