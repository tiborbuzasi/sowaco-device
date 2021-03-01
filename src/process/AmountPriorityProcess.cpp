/**
 * Class for Amount Priority Process
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "AmountPriorityProcess.hpp"

#include <Arduino.h>

#include "../bluetooth/BtUuid.hpp"

AmountPriorityProcess::AmountPriorityProcess(Sensor* sensors[3], Actuator* actuators[4],
    std::unordered_map<const char*, Setting*>* settings, unsigned char goalLoads, unsigned char goalHour)
    : Process(sensors, actuators, settings), goalLoads(goalLoads), goalHour(goalHour) {}

AmountPriorityProcess::~AmountPriorityProcess() {}

void AmountPriorityProcess::updateWaiting()
{
    // Check for big water consumption - process should be redefined
    unsigned char tankLoads = getTankLoads();
    if (tankLoads + 1 < goalLoads)
    {
        finished = true;
        return;
    }

    // Check if collector temperature is under goal and time is not up
    if (!collectorIsHot() && !timeIsUp())
    {
        return;
    }

    // Check if collector is hotter than the tank
    bool collectorIsHotter = collectorAverageTemperatureSensor->getValue()
        > tankTemperatureSensor->getValue() + tankAndCollectorTempDiff;

    // Check if draining the tank is needed (and if it is the first load today)
    if (!loadsToday && ((collectorIsHot() && tankIsCold(tankAndNeededTempDiff))
        || (timeIsUp() && collectorIsHotter)))
    {
        drainTank();
        return;
    }

    // Check if water is still getting warmer (and time is not up and it is the last load today)
    if ((loadsToday == settings->at(CHAR_GOAL_LEVEL)->getValue() - 1) && collectorIsHot() && !timeIsUp())
    {
        if (lastTemperatureValue < collectorAverageTemperatureSensor->getValue())
        {
            unsigned long time = millis();
            // Update last temperature
            if (time - lastTemperatureTime > processWaitingCycleTime - 5000)
            lastTemperatureValue = collectorAverageTemperatureSensor->getValue();
            lastTemperatureTime = time;
            return;
        }
    }

    // Check if tank level is under goal level
    if (getTankLoads() < goalLoads)
    {
        drainPipe();
        return;
    }

    // Check if water should be replaced in tank
    if (getTankLoads() == tankMaxLoads && tankIsCold(fullTankAndNeededTempDiff))
    {
        drainTank(true);
        return;
    }
}

bool AmountPriorityProcess::tankIsCold(unsigned int difference)
{
    // Check if tank temperature and allowed difference is smaller than needed temperature
    if (tankTemperatureSensor->getValue() + difference
        < settings->at(CHAR_GOAL_TEMPERATURE)->getValue() * 100 + calculateCoolingCorrection())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool AmountPriorityProcess::timeIsUp()
{
    // Check if clock time hour is greater than or equal to process goal hour
    if (getTimeHour() >= goalHour)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}