/**
 * Class for Amount Priority Process - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_AMOUNT_PRIORITY_PROCESS_H
#define SOWACO_AMOUNT_PRIORITY_PROCESS_H

#include "Process.hpp"

class AmountPriorityProcess : public Process {
public:
    /**
     * Create amount priority process object
     *
     * @param sensors Array of the needed sensors
     * @param actuators Array of the needed actuators
     * @param settings Pointer to the process settings
     * @param goalLoads The goal loads level to reach in this process
     * @param goalHour The goal hour to reach in this process
     */
    AmountPriorityProcess(Sensor* sensors[3],
        Actuator* actuators[4],
        std::unordered_map<const char*, Setting*>* settings,
        unsigned char goalLoads,
        unsigned char goalHour
    );

    /**
     * Destroy amount priority process object
     */
    ~AmountPriorityProcess();

    /**
     * Update process
     */
    void updateWaiting();

private:
    /**
     * Allowed difference between tank and needed temperature
     */
    const unsigned int tankAndNeededTempDiff = 300;

    /**
     * Allowed difference between (full) tank and needed temperature
     */
    const unsigned int fullTankAndNeededTempDiff = 1000;

    /**
     * Allowed difference between the collector and the needed temperature
     */
    const unsigned int collectorAndNeededTempDiff = 500;

    /**
     * Allowed difference between the tank and the collector temperature
     */
    const unsigned int tankAndCollectorTempDiff = 500;

    /**
     * Process goal loads level for collecting
     */
    unsigned char goalLoads;

    /**
     * Process goal hour for collecting
     */
    unsigned char goalHour;

    /**
     * Last saved temperature value
     */
    unsigned int lastTemperatureValue = 0;

    /**
     * Last saved temperature time
     */
    unsigned long lastTemperatureTime = 0;

    /**
     * Tank temperature is colder than the goal temperature (with difference)
     *
     * @param difference The difference to calculate with
     *
     * @return Tank temperature is cold
     */
    bool tankIsCold(unsigned int difference);

    /**
     * Process has reached its end time
     */
    bool timeIsUp();
};

#endif // SOWACO_AMOUNT_PRIORITY_PROCESS_H
