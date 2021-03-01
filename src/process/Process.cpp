/**
 * Class for Process
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "Process.hpp"

#include "Arduino.h"

#include "../bluetooth/BtUuid.hpp"
#include "../sensors/WaterLevelSensor.hpp"

unsigned char Process::loadsToday = 0;

TimeSetting* Process::timeSetting = nullptr;

Process::Process(Sensor* sensors[3], Actuator* actuators[4], std::unordered_map<const char*, Setting*>* settings)
{
    collectorAverageTemperatureSensor = sensors[0];
    tankTemperatureSensor = sensors[1];
    tankLevelSensor = sensors[2];

    collectorValve = actuators[0];
    drainValve = actuators[1];
    tankValve = actuators[2];
    tankDrainValve = actuators[3];

    this->settings = settings;
}

Process::~Process() {}

bool Process::hasFinished()
{
    return finished;
}

ProcessState Process::getState()
{
    return state;
}

void Process::setState(ProcessState state)
{
    this->state = state;
    stateStartTime = millis();
}

void Process::update()
{
    switch (state)
    {
    case PIPE_DRAINING:
        updatePipeDraining();
        break;
    case COLLECTING:
        updateCollecting();
        break;
    case DRAINING:
        updateDraining();
        break;
    case PARTIAL_DRAINING:
        updatePartialDraining();
        break;
    default:
        updateWaiting();
        break;
    }
}

void Process::updatePipeDraining()
{
    // If time is up change state to collecting
    if (millis() - stateStartTime > settings->at(CHAR_PIPE_DRAINING_TIME)->getValue() * 1000)
    {
        collect();
        return;
    }
}

void Process::updateCollecting()
{
    // Loop until water level is under process goal tank level
    if (tankLevelSensor->getValue() < goalTankLevel) {
        return;
    }

    // If water level was reached change state to waiting
    loadsToday++;
    finished = true;
    wait();
}

void Process::updateDraining()
{
    unsigned long time = millis();

    // Loop until water level is above 5% (and delay state start time)
    if (!tankIsEmpty())
    {
        stateStartTime = time;
        return;
    }

    // If time is up change state to waiting
    if (time + processDutyCycleTime - stateStartTime
    > settings->at(CHAR_TANK_DRAINING_TIME)->getValue() * 1000)
    {
        wait();
        return;
    }
}

void Process::updatePartialDraining()
{
    // If water level has reached 75% change state to pipe draining
    if (tankLevelSensor->getValue() <= 75)
    {
        drainPipe();
        return;
    }
}

bool Process::collectorIsHot()
{
    // Collector temperature is above needed temperature
    if (collectorAverageTemperatureSensor->getValue()
        > settings->at(CHAR_GOAL_TEMPERATURE)->getValue() * 100 + calculateCoolingCorrection())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Process::tankIsEmpty()
{
    // Tank water level is not above 5%
    if (tankLevelSensor->getValue() <= 5)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Process::tankReachedGoal()
{
    // Tank water level is at least at goal level
    if (tankLevelSensor->getValue() >= settings->at(CHAR_GOAL_LEVEL)->getValue())
    {
        return true;
    }
    else
    {
        return false;
    }
}

unsigned char Process::getTankLoads()
{
    // Calculate the number of loads based on the tank water level
    unsigned int tankLevel = this->tankLevelSensor->getValue();

    if (tankLevel <= 15) return 0;
    if (tankLevel > 15 && tankLevel <= 40) return 1;
    if (tankLevel > 40 && tankLevel <= 65) return 2;
    if (tankLevel > 65 && tankLevel <= 90) return 3;
    return 4;
}

unsigned int Process::calculateCoolingCorrection()
{
    // Number of hours left until goal time
    unsigned char hours = settings->at(CHAR_GOAL_TIME)->getValue() - getTimeHour();

    // Number of loads in the tank after collecting
    unsigned char loads = getTankLoads() + 1;

#ifdef DEBUG
    Serial.print("Colling correction (C): ");
    Serial.println(hours / loads * 100);
#endif

    return hours / loads * 100;
}

void Process::wait()
{
    // Set process state
    setState(WAITING);

    // Deactivate all valves
    if (collectorValve->getValue()) collectorValve->setValue(false);
    if (drainValve->getValue()) drainValve->setValue(false);
    if (tankValve->getValue()) tankValve->setValue(false);
    if (tankDrainValve->getValue()) tankDrainValve->setValue(false);

#ifdef DEBUG
    Serial.println("Process state changed to WAITING.");
#endif
}

void Process::drainPipe() {
    // If pipe draining is turned off, skip to collecting state
    if (settings->at(CHAR_PIPE_DRAINING_TIME)->getValue() == 0)
    {
        collect();
        return;
    }

    // Set process state
    setState(PIPE_DRAINING);

    // Activate collector and drain valve, deactivate the others
    if (!collectorValve->getValue()) collectorValve->setValue(true);
    if (!drainValve->getValue()) drainValve->setValue(true);
    if (tankValve->getValue()) tankValve->setValue(false);
    if (tankDrainValve->getValue()) tankDrainValve->setValue(false);

#ifdef DEBUG
    Serial.println("Process state changed to PIPE_DRAINING.");
#endif
}

void Process::collect()
{
    // Set process state
    setState(COLLECTING);

    // Calculate tank level goal for this collecting
    goalTankLevel = tankLevelSensor->getValue() + (100 / tankMaxLoads);
    if (goalTankLevel > 100) goalTankLevel = 100;

    // Activate collector and tank valve, deactivate the others
    if (!collectorValve->getValue()) collectorValve->setValue(true);
    if (drainValve->getValue()) drainValve->setValue(false);
    if (!tankValve->getValue()) tankValve->setValue(true);
    if (tankDrainValve->getValue()) tankDrainValve->setValue(false);

#ifdef DEBUG
    Serial.println("Process state changed to COLLECTING.");
#endif
}

void Process::drainTank(bool partial)
{
    // If tank draining is turned off, skip to draining pipe
    if (!partial && settings->at(CHAR_TANK_DRAINING_TIME)->getValue() == 0)
    {
        drainPipe();
        return;
    }

    // Set process state
    setState(partial ? PARTIAL_DRAINING : DRAINING);

    // Activate tank drain valve, deactivate the others
    if (collectorValve->getValue()) collectorValve->setValue(false);
    if (drainValve->getValue()) drainValve->setValue(false);
    if (tankValve->getValue()) tankValve->setValue(false);
    if (!tankDrainValve->getValue()) tankDrainValve->setValue(true);

#ifdef DEBUG
    Serial.println("Process state changed to DRAINING/PARTIAL DRAINING.");
#endif
}

void Process::init(TimeSetting* timeSetting)
{
    Process::timeSetting = timeSetting;
}

bool Process::checkPlan()
{
    return true;
}

unsigned char Process::getLoadsToday()
{
    return loadsToday;
}

void Process::clearLoadsToday()
{
    loadsToday = 0;
}

unsigned long Process::getTime()
{
    return timeSetting->getValue();
}

unsigned long Process::getTimeHour(time_t time)
{
    if (!time)
    {
        time = getTime();
    }

    tm* timeComponents = localtime(&time);
    return timeComponents->tm_hour;
}