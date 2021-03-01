/**
 * Class for Process Setting - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_PROCESS_SETTING_H
#define SOWACO_PROCESS_SETTING_H

#include "Int8Setting.hpp"

class ProcessSetting : public Int8Setting
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
    ProcessSetting(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        const char* group,
        const char* key,
        uint8_t defaultValue,
        uint32_t permissions = BT_PERMISSION_READ | ESP_GATT_PERM_WRITE_ENC_MITM,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Get new setting value
     * 
     * @return New setting value integer representation
     */
    unsigned char getNewValue();

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
    unsigned char newValue = 0;
};

#endif // SOWACO_PROCESS_SETTING_H