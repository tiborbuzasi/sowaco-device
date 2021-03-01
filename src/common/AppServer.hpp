/**
 * Class for Server - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_APPSERVER_H
#define SOWACO_APPSERVER_H

#include <BLEDevice.h>
#include <unordered_map>

#include "../process/Process.hpp"
#include "../sensors/Sensor.hpp"
#include "../settings/Setting.hpp"
#include "Actuator.hpp"

class AppServer : public BLEServerCallbacks
{
public:
    /**
     * Create new object
     */
    void init();

    /**
     * Update communication
     */
    void update();

    /**
     * Bluetooth server connection handler
     */
    void onConnect(BLEServer* pServer);

    /**
     * Bluetooth server disconnection handler
     */
    void onDisconnect(BLEServer* pServer);

private:
    /**
     * Cycle time of (Bluetooth) device communication
     */
    const unsigned long communicationCycleTime = 1000;

    /**
     * There is a device connected
     */
    bool hasDeviceConnected = false;

    /**
     * Bluetooth server
     */
    BLEServer* server = nullptr;

    /**
     * Bluetooth connection security
     */
    BLESecurity* security = nullptr;

    /**
     * Sensors' service
     */
    BLEService* sensorsService = nullptr;
    std::map<const char*, Sensor*> sensors;

    /**
     * Actuators' service
     */
    BLEService* actuatorsService = nullptr;
    std::unordered_map<const char*, Actuator*> actuators;

    /**
     * Device settings' service
     */
    BLEService* deviceSettingsService = nullptr;
    std::unordered_map<const char*, Setting*> deviceSettings;

    /**
     * Process settings' service
     */
    BLEService* processSettingsService = nullptr;
    std::unordered_map<const char*, Setting*> processSettings;

    /**
     * Process to run
     */
    Process* process = nullptr;

    /**
     * Time of running the last loop
     */
    unsigned long lastLoopTime = 0;

    /**
     * Time of running the last communication loop
     */
    unsigned long lastCommunicationTime = 0;

    /**
     * Initialise Bluetooth security
     */
    void initBtSecurity();

    /**
     * Initialise device settings
     */
    void initDeviceSettings(BLEService* service);

    /**
     * Initialise process settings
     */
    void initProcessSettings(BLEService* service);

    /**
     * Initialise sensors
     */
    void initSensors(BLEService* service);

    /**
     * Initialise actuators
     */
    void initActuators(BLEService* service);

    /**
     * Notify Bluetooth device on change of values
     */
    void notify();

    /**
     * Calculate cycle time
     */
    unsigned int calculateCycleTime();
};

#endif // SOWACO_APPSERVER_H