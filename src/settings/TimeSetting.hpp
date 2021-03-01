/**
 * Class for Time Setting - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_TIME_SETTING_H
#define SOWACO_TIME_SETTING_H

#include "Setting.hpp"

class TimeSetting : public Setting
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
    TimeSetting(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        const char* group,
        const char* key,
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
     * Get system time
     * 
     * @return Current time in seconds
     */
    time_t getTime();

    /**
     * Set system time
     * 
     * @param time Time to set in seconds
     */
    void setTime(time_t);
};

#endif // SOWACO_TIME_SETTING_H