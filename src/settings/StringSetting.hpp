/**
 * Class for String Setting - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_STRING_SETTING_H
#define SOWACO_STRING_SETTING_H

#include "Setting.hpp"

class StringSetting : public Setting
{
public:
    /**
     * Create a new setting object (with string as default value)
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param groupName ESP32 Preferences group
     * @param shortName ESP32 Preferences key
     * @param defaultValue ESP32 Preferences default value
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    StringSetting(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        const char* group,
        const char* key,
        const char* defaultValue,
        uint32_t permissions = BT_PERMISSION_READ | ESP_GATT_PERM_WRITE_ENC_MITM,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Get setting value as string
     *
     * @return Setting value string representation
     */
    const char* getStringValue();

    /**
     * Set setting value as integer
     * 
     * @param value Value to assign to the setting
     */
    void setValue(const char* value);

    /**
     * On Bluetooth write event handler
     *
     * @param characteristic Related characteristic
     */
    void onWrite(BLECharacteristic* characteristic);

private:
    /**
     * Setting string value
     */
    const char* value = nullptr;
};

#endif // SOWACO_SETTING_H