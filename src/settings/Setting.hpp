/**
 * Class for Setting - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_SETTING_H
#define SOWACO_SETTING_H

#include "../bluetooth/BtCharacteristic.hpp"
#include "../app.hpp"

class Setting : public BtCharacteristic
{
public:
    /**
     * Create a new setting object
     *
     * @param service Bluetooth service
     * @param uuid Bluetooth characteristic ID
     * @param name Characteristic name
     * @param type Characteristic data type
     * @param groupName ESP32 Preferences group
     * @param shortName ESP32 Preferences key
     * @param storable Setting should be permanently stored
     * @param permissions Bitmask of permissions
     * @param properties Bitmask of properties
     */
    Setting(
        BLEService* service,
        const char* uuid,
        const char* name,
        DataType type,
        const char* group,
        const char* key,
        bool storable = true,
        uint32_t permissions = BT_PERMISSION_READ | ESP_GATT_PERM_WRITE_ENC_MITM,
        uint32_t properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE_NR | BLECharacteristic::PROPERTY_NOTIFY
    );

    /**
     * Get setting value as 8-bit integer
     *
     * @return Setting value integer representation
     */
    virtual unsigned char getValue();

    /**
     * Get setting value as 32-bit integer
     *
     * @return Setting value integer representation
     */
    virtual unsigned int getIntValue();

    /**
     * Get setting value as string
     * 
     * @return Setting value string representation
     */
    virtual const char* getStringValue();

    /**
     * Set setting value as 8-bit integer
     * 
     * @param value Value to assign to the setting
     */
    virtual void setValue(unsigned char value);

    /**
     * Set setting value as 32-bit integer
     * 
     * @param value Value to assign to the setting
     */
    virtual void setValue(unsigned int value);

    /**
     * Set setting value as string
     * 
     * @param value Value to assign to the setting
     */
    virtual void setValue(const char* value);

    /**
     * Bluetooth write event handler
     *
     * @param characteristic Related characteristic
     */
    virtual void onWrite(BLECharacteristic* characteristic) = 0;

protected:
    /**
     * Setting group name
     */
    const char* group;

    /**
     * Setting key
     */
    const char* key;

    /**
     * Setting type
     */
    DataType type;

    /**
     * Setting should be stored
     */
    bool storable = true;
};

#endif // SOWACO_SETTING_H