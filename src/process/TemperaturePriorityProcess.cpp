/**
 * Class for Temperature Priority Process
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "TemperaturePriorityProcess.hpp"

#include "../bluetooth/BtUuid.hpp"

TemperaturePriorityProcess::TemperaturePriorityProcess(Sensor* sensors[3], Actuator* actuators[4],
    std::unordered_map<const char*, Setting*>* settings)
    : Process(sensors, actuators, settings) {}

TemperaturePriorityProcess::~TemperaturePriorityProcess() {}

void TemperaturePriorityProcess::updateWaiting()
{
    // Check if collector temperature is under goal temperature
    if (!collectorIsHot())
    {
        return;
    }

    // Check if tank temperature is under goal temperature (and if it is the first load today)
    if (!loadsToday && tankIsCold())
    {
        drainTank();
        return;
    }

    // Check if tank level is under goal level
    if (getTankLoads() < settings->at(CHAR_GOAL_LEVEL)->getValue())
    {
        drainPipe();
        return;
    }
}

bool TemperaturePriorityProcess::tankIsCold()
{
    // Check if tank temperature and allowed difference is smaller than needed temperature
    if (tankTemperatureSensor->getValue() + tankAndNeededTempDiff
        < settings->at(CHAR_GOAL_TEMPERATURE)->getValue() * 100 + calculateCoolingCorrection())
    {
        return true;
    }
    else
    {
        return false;
    }
}