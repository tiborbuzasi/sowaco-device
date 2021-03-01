/**
 * Class for Temperature Priority Process - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_TEMPERATURE_PRIORITY_PROCESS_H
#define SOWACO_TEMPERATURE_PRIORITY_PROCESS_H

#include "Process.hpp"

class TemperaturePriorityProcess : public Process
{
public:
    /**
     * Create temperature priority process object
     *
     * @param sensors Array of the needed sensors
     * @param actuators Array of the needed actuators
     * @param settings Pointer to the process settings
     */
    TemperaturePriorityProcess(Sensor* sensors[3], Actuator* actuators[4], std::unordered_map<const char*, Setting*>* settings);

    /**
     * Destroy temperature priority process object
     */
    ~TemperaturePriorityProcess();

    /**
     * Update process in waiting state
     */
    void updateWaiting();

private:
    /**
     * Allowed difference between tank and needed temperature
     */
    const unsigned int tankAndNeededTempDiff = 300;

    /**
     * Tank temperature is colder than the goal temperature (with offset)
     *
     * @return Tank temperature is cold
     */
    bool tankIsCold();
};

#endif // SOWACO_TEMPERATURE_PRIORITY_PROCESS_H
