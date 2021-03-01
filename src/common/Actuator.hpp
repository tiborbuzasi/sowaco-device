/**
 * Class for Actuator - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_ACTUATOR_H
#define SOWACO_ACTUATOR_H

#include <BLEService.h>

#include "../bluetooth/BtCharacteristic.hpp"
#include "../app.hpp"

class Actuator : public BtCharacteristic
{
public:
    /**
     * Create a new actuator object
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param pin GPIO pin
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    Actuator(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        unsigned char pin,
        uint32_t permissions = BT_PERMISSION_READ,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Get actuator value
     *
     * @return Measured value
     */
    bool getValue();

    /**
     * Set actuator value
     */
    void setValue(bool value);

private:
    /**
     * Actuator value
     */
    bool value = false;

    /**
     * GPIO pin
     */
    unsigned char pin;
};

#endif // SOWACO_ACTUATOR_H