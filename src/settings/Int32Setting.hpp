/**
 * Class for 32-bit Integer Setting - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_INT32_SETTING_H
#define SOWACO_INT32_SETTING_H

#include "Setting.hpp"

class Int32Setting : public Setting
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
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    Int32Setting(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        const char* group,
        const char* key,
        uint32_t defaultValue,
        uint32_t permissions = BT_PERMISSION_READ | ESP_GATT_PERM_WRITE_ENC_MITM,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Get setting value
     * 
     * @return Setting value integer representation
     */
    unsigned int getIntValue();

    /**
     * Set setting value
     * 
     * @param value Value to assign to the setting
     */
    void setValue(unsigned int value);

    /**
     * Bluetooth write event handler
     *
     * @param characteristic Related characteristic
     */
    void onWrite(BLECharacteristic* characteristic);

private:
    /**
     * Setting integer value
     */
    unsigned int value = 0;
};

#endif // SOWACO_INT32_SETTING_H