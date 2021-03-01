/**
 * Class for Bluetooth Descriptors - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_BTCHARACTERISTIC_H
#define SOWACO_BTCHARACTERISTIC_H

#include <BLEService.h>

#include <array>

#include "../common/DataType.hpp"

class BtCharacteristic : public BLECharacteristicCallbacks
{
public:
    /**
     * Send notification on Bluetooth characteristic value
     */
    virtual void notify();

protected:
    /**
     * Bluetooth LE characteristic UUID
     */
    const char* uuid;

    /**
     * Bluetooth characteristic
     */
    BLECharacteristic* characteristic = nullptr;

    /**
     * Bluetooth descriptors
     */
    std::array<BLEDescriptor*, 3> descriptors;

    /**
     * Bleutooth notification needed
     */
    bool notificationNeeded = false;

    /**
     * Create Bluetooth charateristic with descriptors
     *
     * @param service The Bluetooth service to add the characteristic to
     * @param uuid Bluetooth characteristic UUID
     * @param name Display name of the characteristic
     * @param type Value type of the characteristic
     * @param permissions Bluetooth characteristic accessibility permissions
     * @param properties Bluetooth characteristic properties
     */
    BtCharacteristic(BLEService* service, const char* uuid, const char* name, DataType type, uint32_t permissions, uint32_t properties);

    /**
     * Create and add Characteristic User Description GATT Descriptor (0x2901) to characteristic
     *
     * @param characteristic The characteristic to add the descriptor to
     * @param name The description of the characteristic
     * @param permissions Bluetooth descriptor accessibility permissions
     */
    virtual void addDescriptor2901(BLECharacteristic* characteristic, const char* name, uint32_t permissions);

    /**
     * Create and add Client Characteristic Configuration GATT Descriptor (0x2902) to characteristic
     *
     * @param characteristic The characteristic to add the descriptor to
     * @param notification The characteristic must be notified
     * @param indication The characteristic must be indicated
     * @param permissions Bluetooth descriptor accessibility permissions
     */
    virtual void addDescriptor2902(BLECharacteristic* characteristic, bool notification = false, bool indication = false, uint32_t permissions = 0);

    /**
     * Create and add Characteristic Presentation Format GATT Descriptor (0x2904) to characteristic
     *
     * @param characteristic The characteristic to add the descriptor to
     * @param type The format of the characteristic
     * @param permissions Bluetooth descriptor accessibility permissions
     */
    virtual void addDescriptor2904(BLECharacteristic* characteristic, DataType type, uint32_t permissions);
};

#endif // SOWACO_BTCHARACTERISTIC_H