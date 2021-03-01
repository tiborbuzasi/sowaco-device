/**
 * Class for 8-bit Integer Setting - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_INT8_SETTING_H
#define SOWACO_INT8_SETTING_H

#include "Setting.hpp"

class Int8Setting : public Setting
{
public:
    /**
     * Create a new setting object
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param group ESP32 Preferences group
     * @param short ESP32 Preferences key
     * @param defaultValue ESP32 Preferences default value
     * @param storable Setting should be permanently stored
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    Int8Setting(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        const char* group,
        const char* key,
        uint8_t defaultValue,
        bool storable = true,
        uint32_t permissions = BT_PERMISSION_READ | ESP_GATT_PERM_WRITE_ENC_MITM,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Get setting value
     * 
     * @return Setting value
     */
    unsigned char getValue();

    /**
     * Set setting value
     * 
     * @param value Value to assign to the setting
     */
    void setValue(unsigned char value);

    /**
     * Bluetooth write event handler
     *
     * @param characteristic Related characteristic
     */
    void onWrite(BLECharacteristic* characteristic);

protected:
    /**
     * Setting integer value
     */
    unsigned char value;
};

#endif // SOWACO_INT8_SETTING_H