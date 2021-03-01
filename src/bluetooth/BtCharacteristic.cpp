/**
 * Class for Bluetooth Descriptors
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "BtCharacteristic.hpp"

#include <Arduino.h>
#include <BLE2902.h>
#include <BLE2904.h>

#include "BtUuid.hpp"

BtCharacteristic::BtCharacteristic(BLEService* service, const char* uuid, const char* name, DataType type, uint32_t permissions, uint32_t properties)
{
    this->uuid = uuid;
    this->characteristic = service->createCharacteristic(uuid, properties);
    this->characteristic->setAccessPermissions(permissions);
    this->characteristic->setCallbacks(this);
    delay(100);

    // Add Characteristic User Description GATT Descriptor (0x2901)
    addDescriptor2901(this->characteristic, name, permissions);
    delay(100);

    // Add Client Characteristic Configuration GATT Descriptor (0x2902)
    addDescriptor2902(this->characteristic, true, false, permissions);
    delay(100);

    // Add add Characteristic Presentation Format GATT Descriptor (0x2904)
    addDescriptor2904(this->characteristic, type, permissions);
    delay(100);
}

void BtCharacteristic::addDescriptor2901(BLECharacteristic* characteristic, const char* name, uint32_t permissions)
{
    BLEDescriptor* descriptor = new BLEDescriptor(DESCRIPTOR_USER_DESCRIPTION);

    descriptor->setAccessPermissions(permissions);
    descriptor->setValue(name);

    descriptors[0] = descriptor;
    characteristic->addDescriptor(descriptor);
}

void BtCharacteristic::addDescriptor2902(BLECharacteristic* characteristic, bool notification, bool indication, uint32_t permissions)
{
    BLE2902* descriptor = new BLE2902();

    descriptor->setAccessPermissions(permissions);
    descriptor->setNotifications(notification);
    descriptor->setIndications(indication);

    descriptors[1] = descriptor;
    characteristic->addDescriptor(descriptor);
}

void BtCharacteristic::addDescriptor2904(BLECharacteristic* characteristic, DataType type, uint32_t permissions)
{
    BLE2904* descriptor = new BLE2904();

    descriptor->setAccessPermissions(permissions);
    switch (type)
    {
    case STRING:
        descriptor->setFormat(BLE2904::FORMAT_UTF8);
        descriptor->setUnit(0x2700);
        break;
    case INTEGER:
        descriptor->setFormat(BLE2904::FORMAT_UINT8);
        descriptor->setUnit(0x2700);
        break;
    case BOOLEAN:
        descriptor->setFormat(BLE2904::FORMAT_BOOLEAN);
        descriptor->setUnit(0x2700);
        break;
    case CELSIUS:
        descriptor->setExponent(2);
        descriptor->setFormat(BLE2904::FORMAT_UINT8);
        descriptor->setUnit(0x272F);
        break;
    case MILLIMETER:
        descriptor->setExponent(-3);
        descriptor->setFormat(BLE2904::FORMAT_UINT8);
        descriptor->setUnit(0x2701);
        break;
    case MILLISECONDS:
        descriptor->setExponent(-3);
        descriptor->setFormat(BLE2904::FORMAT_UINT32);
        descriptor->setUnit(0x2703);
        break;
    case PERCENT:
        descriptor->setFormat(BLE2904::FORMAT_SINT8);
        descriptor->setUnit(0x27AD);
        break;
    }

    descriptors[2] = descriptor;
    characteristic->addDescriptor(descriptor);
}

void BtCharacteristic::notify()
{
    if (notificationNeeded)
    {
        notificationNeeded = false;
        characteristic->notify();
    }
}