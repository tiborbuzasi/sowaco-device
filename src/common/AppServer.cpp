/**
 * Class for Server
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "AppServer.hpp"

#include <Arduino.h>

#include "../bluetooth/BtUuid.hpp"
#include "../process/AmountPriorityProcess.hpp"
#include "../process/OperationMode.hpp"
#include "../process/TemperaturePriorityProcess.hpp"
#include "../sensors/AverageTemperatureSensor.hpp"
#include "../sensors/TemperatureSensor.hpp"
#include "../sensors/TemperatureSensorsSerialCodes.hpp"
#include "../sensors/UltrasonicSensor.hpp"
#include "../sensors/UltrasonicDistanceSensor.hpp"
#include "../sensors/WaterLevelSensor.hpp"
#include "../settings/Int8Setting.hpp"
#include "../settings/Int32Setting.hpp"
#include "../settings/ProcessSetting.hpp"
#include "../settings/StringSetting.hpp"
#include "../settings/TimeSetting.hpp"
#include "../app.hpp"
#include "../gpio.hpp"
#include "DataType.hpp"
#include "Settings.hpp"

void AppServer::init()
{
    // Initialise and create bluetooth device
    BLEDevice::init(Settings::readString("device", "name", DEVICE_NAME));
    while (!BLEDevice::getInitialized()) {}
    server = BLEDevice::createServer();
    server->setCallbacks(this);

    // Initialise security
    initBtSecurity();

    // Create device settings' service
    deviceSettingsService = server->createService(BLEUUID(SERVICE_DEVICE_SETTINGS), 41);
    initDeviceSettings(deviceSettingsService);

    // Create process settings' service
    processSettingsService = server->createService(BLEUUID(SERVICE_PROCESS_SETTINGS), 41);
    initProcessSettings(processSettingsService);

    // Create sensors' service
    sensorsService = server->createService(BLEUUID(SERVICE_SENSORS), 41);
    initSensors(sensorsService);

    // Create actuators' service
    actuatorsService = server->createService(BLEUUID(SERVICE_ACTUATORS), 21);
    initActuators(actuatorsService);

    // Start services
    deviceSettingsService->start();
    processSettingsService->start();
    sensorsService->start();
    actuatorsService->start();
    delay(1000);

    // Setup and start advertising
    BLEAdvertising* advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_DEVICE_SETTINGS);
    advertising->addServiceUUID(SERVICE_PROCESS_SETTINGS);
    advertising->addServiceUUID(SERVICE_SENSORS);
    advertising->addServiceUUID(SERVICE_ACTUATORS);
    BLEDevice::startAdvertising();

    // Setup process
    TimeSetting* timeSetting = static_cast<TimeSetting*>(deviceSettings.at(CHAR_DEVICE_TIME));
    Process::init(timeSetting);
}

void AppServer::update()
{
    // Check if restart is required
    if (deviceSettings.at(CHAR_DEVICE_RESTART)->getValue())
    {
        ESP.restart();
    }

    // Check if reset is required
    if (deviceSettings.at(CHAR_DEVICE_RESET)->getValue())
    {
        Settings::resetToFactory();
        ESP.restart();
    }

    // Check if applying new collecting plan is required
    if (deviceSettings.at(CHAR_DEVICE_APPLY_NEW_PLAN)->getValue() == 1)
    {
        // Set return value to succeeded
        unsigned char returnValue = 2;

        // Check plan
        if (Process::checkPlan())
        {
            // Check process state
            if (!process || process->getState() == WAITING)
            {
                // Reset process
                if (process)
                {
                    delete process;
                    process = nullptr;
                }

                // Apply new settings
                for (auto i = processSettings.begin(); i != processSettings.end(); i++)
                {
                    ProcessSetting* setting = static_cast<ProcessSetting*>(i->second);
                    setting->setValue(setting->getNewValue());
                }
            }
            else
            {
                returnValue = 4; // Process is not in waiting mode
            }
        }
        else
        {
            returnValue = 3; // Invalid plan
        }

        // Returning reponse
        Int8Setting* applyNewPlan = static_cast<Int8Setting*>(deviceSettings.at(CHAR_DEVICE_APPLY_NEW_PLAN));
        applyNewPlan->setValue(returnValue);
    }

    // Check automation mode
    bool automatic = deviceSettings.at(CHAR_AUTOMATION_MODE)->getValue();
    if (automatic)
    {
        // Check if current time is inside of the operation time
        unsigned char currentHour = Process::getTimeHour();
        if (processSettings.at(CHAR_OPERATION_START_TIME)->getValue() >= currentHour)
        {
            // Delete process if it has ended
            if (process && process->hasFinished())
            {
                delete process;
                process = nullptr;
            }

            // Create process if it does not exist and operation time has not ended
            if (!process && processSettings.at(CHAR_OPERATION_END_TIME)->getValue() > currentHour) {

                Sensor* sensors[3] = {
                    this->sensors.at(CHAR_COLLECTOR_AVERAGE_TEMPERATURE),
                    this->sensors.at(CHAR_TANK_TEMPERATURE),
                    this->sensors.at(CHAR_TANK_LEVEL_PERCENT)
                };
                Actuator* actuators[4] = {
                    this->actuators.at(CHAR_COLLECTOR_VALVE),
                    this->actuators.at(CHAR_DRAIN_VALVE),
                    this->actuators.at(CHAR_TANK_VALVE),
                    this->actuators.at(CHAR_TANK_DRAIN_VALVE)
                };
                switch (processSettings.at(CHAR_OPERATION_MODE)->getValue())
                {
                case TEMPERATURE_PRIORITY:
                    process = new TemperaturePriorityProcess(sensors, actuators, &processSettings);

#ifdef DEBUG
                    Serial.println("Starting Temperature priority process...");
#endif

                    break;
                case AMOUNT_PRIORITY:
                    unsigned char currentHour = Process::getTimeHour(0);
                    // Divider should be the subtract of maxLoads - currentLoads, but divide-by-0 is possible
                    unsigned char processLength = (processSettings.at(CHAR_GOAL_TIME)->getValue() - currentHour) / Process::tankMaxLoads;

                    process = new AmountPriorityProcess(sensors, actuators, &processSettings, Process::getLoadsToday() + 1, currentHour + processLength);

#ifdef DEBUG
                    Serial.println("Starting Amount priority process...");
#endif

                    break;
                }
            }
        }
    }

    // Loop if needed (reading sensor values, Bluetooth communication)
    if (automatic || hasDeviceConnected)
    {
        unsigned long time = millis();
        if (time - lastLoopTime > calculateCycleTime()) {
            lastLoopTime = time;

            // Update sensor values
            for (auto i = sensors.begin(); i != sensors.end(); i++)
            {
                i->second->update();
            }

            // Update process
            if (process)
            {
                process->update();
            }

            // Check if device is connected
            if (hasDeviceConnected && time - lastCommunicationTime > communicationCycleTime)
            {
                lastCommunicationTime = time;

                // Update time setting
                deviceSettings.at(CHAR_DEVICE_TIME)->setValue((unsigned int)0);

                // Notify Bluetooth device of changed values
                notify();
            }

            // Check if it is midnight and reset loads today
            if (Process::getLoadsToday()) {
                Process::clearLoadsToday();
            }

#ifdef DEBUG
            Serial.println();
#endif
        }
    }
}

// Based on https://github.com/nkolban/esp32-snippets/issues/793#issuecomment-458972303 as at 10.11.2020.
void AppServer::initBtSecurity()
{
#ifdef DEBUG
    Serial.print("Initialising security... ");
#endif

    // Set encryption and passcode
    BLEDevice::setEncryptionLevel((esp_ble_sec_act_t)BT_ENCRYPTION);
    security = new BLESecurity();
    security->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM);
    security->setCapability(ESP_IO_CAP_OUT);
    uint32_t passKey = Settings::readUint32("device", "passKey", DEVICE_PASSKEY);

#ifdef DEBUG
    Serial.print("Passkey: ");
    Serial.println(passKey);
#endif

    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passKey, sizeof(uint32_t));

#ifdef DEBUG
    Serial.println("DONE");
#endif
}

void AppServer::initDeviceSettings(BLEService* service)
{
#ifdef DEBUG
    Serial.print("Initialising device settings... ");
#endif

    // Initialise Device Name
    deviceSettings.emplace(std::make_pair(
        CHAR_DEVICE_NAME,
        new StringSetting(service, CHAR_DEVICE_NAME, "Device Name", DataType::STRING, "device", "name", DEVICE_NAME)));

    // Initialise Device PassKey
    deviceSettings.emplace(std::make_pair(
        CHAR_DEVICE_PASSKEY,
        new Int32Setting(service, CHAR_DEVICE_PASSKEY, "Unknown", DataType::INTEGER, "device", "passKey", DEVICE_PASSKEY, ESP_GATT_PERM_WRITE_ENC_MITM, BLECharacteristic::PROPERTY_WRITE_NR)));

    // Initialise Apply New Plan (Confirm the change of Process Parameters)
    deviceSettings.emplace(std::make_pair(
        CHAR_DEVICE_APPLY_NEW_PLAN,
        new Int8Setting(service, CHAR_DEVICE_APPLY_NEW_PLAN, "Unknown", DataType::INTEGER, "device", "applyNewPlan", 0, false)));

    // Initialise Device Restart
    deviceSettings.emplace(std::make_pair(
        CHAR_DEVICE_RESTART,
        new Int8Setting(service, CHAR_DEVICE_RESTART, "Unknown", DataType::BOOLEAN, "device", "restart", 0, false)));

    // Initialise Device Reset
    deviceSettings.emplace(std::make_pair(
        CHAR_DEVICE_RESET,
        new Int8Setting(service, CHAR_DEVICE_RESET, "Unknown", DataType::BOOLEAN, "device", "reset", 0, false)));

    // Initialise Device Time
    deviceSettings.emplace(std::make_pair(
        CHAR_DEVICE_TIME,
        new TimeSetting(service, CHAR_DEVICE_TIME, "Unknown", DataType::INTEGER, "device", "time")));

    // Initialise Automation Mode
    deviceSettings.emplace(std::make_pair(
        CHAR_AUTOMATION_MODE,
        new Int8Setting(service, CHAR_AUTOMATION_MODE, "Automation Mode", DataType::BOOLEAN, "device", "automationMode", 0, false)));

    // Initialise Process State
    deviceSettings.emplace(std::make_pair(
        CHAR_PROCESS_STATE,
        new Int8Setting(service, CHAR_PROCESS_STATE, "Process State", DataType::BOOLEAN, "process", "state", 0, false)));

#ifdef DEBUG
    Serial.println("DONE");
#endif
}

void AppServer::initProcessSettings(BLEService* service)
{
#ifdef DEBUG
    Serial.print("Initialising process settings... ");
#endif

    // Initialise Operation Mode
    processSettings.emplace(std::make_pair(
        CHAR_OPERATION_MODE,
        new ProcessSetting(service, CHAR_OPERATION_MODE, "Operation Mode", DataType::INTEGER, "process", "operationMode", OPERATION_MODE)));

    // Initialise Operation Start Time
    processSettings.emplace(std::make_pair(
        CHAR_OPERATION_START_TIME,
        new ProcessSetting(service, CHAR_OPERATION_START_TIME, "Operation Start Time", DataType::INTEGER, "process", "opStartTime", OPERATION_START_TIME)));

    // Initialise Operation End Time
    processSettings.emplace(std::make_pair(
        CHAR_OPERATION_END_TIME,
        new ProcessSetting(service, CHAR_OPERATION_END_TIME, "Operation End Time", DataType::INTEGER, "process", "opEndTime", OPERATION_END_TIME)));

    // Initialise Goal Level
    processSettings.emplace(std::make_pair(
        CHAR_GOAL_LEVEL,
        new ProcessSetting(service, CHAR_GOAL_LEVEL, "Goal Level", DataType::INTEGER, "process", "goalLevel", GOAL_LEVEL)));

    // Initialise Goal Temperature
    processSettings.emplace(std::make_pair(
        CHAR_GOAL_TEMPERATURE,
        new ProcessSetting(service, CHAR_GOAL_TEMPERATURE, "Goal Temperature", DataType::INTEGER, "process", "goalTemperature", GOAL_TEMPERATURE)));

    // Initialise Goal Time
    processSettings.emplace(std::make_pair(
        CHAR_GOAL_TIME,
        new ProcessSetting(service, CHAR_GOAL_TIME, "Goal Time", DataType::INTEGER, "process", "goalTime", GOAL_TIME)));

    // Initialise Pipe Draining Time
    processSettings.emplace(std::make_pair(
        CHAR_PIPE_DRAINING_TIME,
        new ProcessSetting(service, CHAR_PIPE_DRAINING_TIME, "Pipe Draining Time", DataType::INTEGER, "process", "pipeDrTime", PIPE_DRAINING_TIME)));

    // Initialise Collecting Time
    processSettings.emplace(std::make_pair(
        CHAR_TANK_DRAINING_TIME,
        new ProcessSetting(service, CHAR_TANK_DRAINING_TIME, "Tank Draining Time", DataType::INTEGER, "process", "tankDrTime", TANK_DRAINING_TIME)));

#ifdef DEBUG
    Serial.println("DONE");
#endif
}

void AppServer::initSensors(BLEService* service)
{
#ifdef DEBUG
    Serial.print("Initialising sensors... ");
#endif

    // Initialise temperature pack 1
    OneWire* oneWire1 = new OneWire(PIN_TEMPERATURE_1_2_3);
    DallasTemperature* sensorPack1 = new DallasTemperature(oneWire1);
    sensorPack1->setResolution(9);

    // Initialise Collector Bottom Temperature sensor
    TemperatureSensor* temperatureSensor1 = new TemperatureSensor(sensorsService, CHAR_COLLECTOR_BOTTOM_TEMPERATURE, "Collector Bottom Temperature", DataType::CELSIUS, sensorPack1, sensor1SerialCode);
    sensors.emplace(std::make_pair(CHAR_COLLECTOR_BOTTOM_TEMPERATURE, temperatureSensor1));

    // Initialise Collector Middle Temperature sensor
    TemperatureSensor* temperatureSensor2 = new TemperatureSensor(sensorsService, CHAR_COLLECTOR_MIDDLE_TEMPERATURE, "Collector Middle Temperature", DataType::CELSIUS, sensorPack1, sensor2SerialCode);
    sensors.emplace(std::make_pair(CHAR_COLLECTOR_MIDDLE_TEMPERATURE, temperatureSensor2));

    // Initialise Collector Bottom Temperature sensor
    TemperatureSensor* temperatureSensor3 = new TemperatureSensor(sensorsService, CHAR_COLLECTOR_TOP_TEMPERATURE, "Collector Top Temperature", DataType::CELSIUS, sensorPack1, sensor3SerialCode);
    sensors.emplace(std::make_pair(CHAR_COLLECTOR_TOP_TEMPERATURE, temperatureSensor3));

    // Initialise Collector Average Temperature Sensor
    TemperatureSensor* temperatureSensors[3] = { temperatureSensor2, temperatureSensor1, temperatureSensor3 };
    sensors.emplace(std::make_pair(CHAR_COLLECTOR_AVERAGE_TEMPERATURE,
        new AverageTemperatureSensor(sensorsService, CHAR_COLLECTOR_AVERAGE_TEMPERATURE, "Collector Average Temperature", DataType::CELSIUS, temperatureSensors)));


    // Initialise temperature pack 2
    OneWire* oneWire2 = new OneWire(PIN_TEMPERATURE_4);
    DallasTemperature* sensorPack2 = new DallasTemperature(oneWire2);
    sensorPack2->setResolution(9);

    // Initialise Tank Temperature sensor
    TemperatureSensor* tankTemperatureSensor = new TemperatureSensor(sensorsService, CHAR_TANK_TEMPERATURE, "Tank Temperature", DataType::CELSIUS, sensorPack2, sensor4SerialCode);
    sensors.emplace(std::make_pair(CHAR_TANK_TEMPERATURE, tankTemperatureSensor));


    // Initialise Tank Level Delay sensor
    UltrasonicSensor* ultrasonicSensor = new UltrasonicSensor(sensorsService, CHAR_TANK_LEVEL_DELAY, "Tank Level Delay", DataType::MILLISECONDS, PIN_ULTRASONIC_TRIGGER, PIN_ULTRASONIC_ECHO);
    sensors.emplace(std::make_pair(CHAR_TANK_LEVEL_DELAY, ultrasonicSensor));
    pinMode(PIN_ULTRASONIC_ECHO, INPUT);
    pinMode(PIN_ULTRASONIC_TRIGGER, OUTPUT);

    // Initialise Tank Level Distance sensor
    UltrasonicDistanceSensor* ultrasonicDistanceSensor = new UltrasonicDistanceSensor(sensorsService, CHAR_TANK_LEVEL_DISTANCE, "Tank Level Distance", DataType::MILLIMETER, ultrasonicSensor, temperatureSensor1);
    sensors.emplace(std::make_pair(CHAR_TANK_LEVEL_DISTANCE, ultrasonicDistanceSensor));

    // Initialise Tank Level Percent sensor
    sensors.emplace(std::make_pair(CHAR_TANK_LEVEL_PERCENT,
        new WaterLevelSensor(sensorsService, CHAR_TANK_LEVEL_PERCENT, "Tank Level Distance", DataType::PERCENT, ultrasonicDistanceSensor)));

#ifdef DEBUG
    Serial.println("DONE");
#endif
}

void AppServer::initActuators(BLEService* service)
{
#ifdef DEBUG
    Serial.print("Initialising actuators... ");
#endif

    // Initialise Collector Valve
    actuators.emplace(std::make_pair(
        CHAR_COLLECTOR_VALVE,
        new Actuator(service, CHAR_COLLECTOR_VALVE, "Collector Valve", DataType::BOOLEAN, PIN_VALVE_1)));

    // Initialise Tank Valve
    actuators.emplace(std::make_pair(
        CHAR_TANK_VALVE,
        new Actuator(service, CHAR_TANK_VALVE, "Tank Valve", DataType::BOOLEAN, PIN_VALVE_2)));

    // Initialise Drain Valve
    actuators.emplace(std::make_pair(
        CHAR_DRAIN_VALVE,
        new Actuator(service, CHAR_DRAIN_VALVE, "Drain Valve", DataType::BOOLEAN, PIN_VALVE_3)));

    // Initialise Tank Drain Valve
    actuators.emplace(std::make_pair(
        CHAR_TANK_DRAIN_VALVE,
        new Actuator(service, CHAR_TANK_DRAIN_VALVE, "Tank Drain Valve", DataType::BOOLEAN, PIN_VALVE_3)));

#ifdef DEBUG
    Serial.println("DONE");
#endif
}

void AppServer::notify()
{
    // Notification of sensor values
    for (auto i = sensors.begin(); i != sensors.end(); i++)
    {
        i->second->notify();
        delay(15);
    }

    // Notification of actuator values
    for (auto i = actuators.begin(); i != actuators.end(); i++)
    {
        i->second->notify();
        delay(15);
    }

    // Notification of device setting values
    for (auto i = deviceSettings.begin(); i != deviceSettings.end(); i++)
    {
        i->second->notify();
        delay(15);
    }

    // Notification of process setting values
    for (auto i = processSettings.begin(); i != processSettings.end(); i++)
    {
        i->second->notify();
        delay(15);
    }
}

unsigned int AppServer::calculateCycleTime()
{
    // Calculate the lowest needed cycle time
    unsigned int value = Process::processWaitingCycleTime;

    if (Process::processDutyCycleTime < value && process && process->getState() != WAITING)
    {
        value = Process::processDutyCycleTime;
    }

    if (communicationCycleTime < value && hasDeviceConnected)
    {
        value = communicationCycleTime;
    }

    return value;
}

void AppServer::onConnect(BLEServer* server)
{
    hasDeviceConnected = true;
}

void AppServer::onDisconnect(BLEServer* server) {
    hasDeviceConnected = false;
    delay(1000);
    BLEDevice::startAdvertising();

#ifdef DEBUG
    Serial.println("Advertising...");
#endif
}