/**
 * Class for Process - Header File
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#ifndef SOWACO_PROCESS_H
#define SOWACO_PROCESS_H

#include <unordered_map>

#include "../common/Actuator.hpp"
#include "../sensors/Sensor.hpp"
#include "../settings/TimeSetting.hpp"
#include "ProcessState.hpp"

class Process {
public:
    /**
     * Setting for time
     */
    static TimeSetting* timeSetting;

    /**
     * Maximum number of loads (collecting) the tank can store
     */
    static const unsigned char tankMaxLoads = 4;

    /**
     * Cycle time of process in waiting state
     */
    static const unsigned long processWaitingCycleTime = 300000;

    /**
     * Cycle time of process in other states
     */
    static const unsigned long processDutyCycleTime = 500;

    /**
     * Initialise
     *
     * @param timeSetting Pointer to time setting
     */
    static void init(TimeSetting* timeSetting);

    /**
     * Check validity of the new plan
     *
     * @return Plan is valid
     */
    static bool checkPlan();

    /**
     * Get loads of today
     */
    static unsigned char getLoadsToday();

    /**
     * Clear loads of today
     */
    static void clearLoadsToday();

    /**
     * Get actual time
     *
     * @return Actual time
     */
    static unsigned long getTime();

    /**
     * Get hour of the given time
     *
     * Actual time is used if no parameter is given.
     *
     * @param time Time
     *
     * @return Hour value of the given time
     */
    static unsigned long getTimeHour(time_t time = 0);

    /**
     * Create process object
     *
     * @param sensors Array of the needed sensors
     * @param actuators Array of the needed actuators
     * @param settings Pointer to the process settings
     */
    Process(Sensor* sensors[3], Actuator* actuators[4], std::unordered_map<const char*, Setting*>* settings);

    /**
     * Destroy process object
     */
    virtual ~Process();

    /**
     * Process has finished
     *
     * @return Process has finished
     */
    bool hasFinished();

    /**
     * Get process state
     *
     * @return Process state
     */
    ProcessState getState();

    /**
     * Set process state
     *
     * @param The process state to be set
     */
    void setState(ProcessState state);

    /**
     * Update process
     */
    void update();

    /**
     * Update process in waiting state
     */
    virtual void updateWaiting() = 0;

    /**
     * Update process in pipe draining state
     */
    virtual void updatePipeDraining();

    /**
     * Update process in collecting state
     */
    virtual void updateCollecting();

    /**
     * Update process in tank draining state
     */
    virtual void updateDraining();

    /**
     * Update process in partial tank draining state
     */
    virtual void updatePartialDraining();

protected:
    /**
     * Number of loads collected today
     */
    static unsigned char loadsToday;

    /**
     * Process finished
     */
    bool finished = false;

    /**
     * Process state
     */
    ProcessState state = WAITING;

    /**
     * Process state start time
     */
    unsigned long stateStartTime = 0;

    /**
     * Process tank level goal for collecting
     */
    unsigned char goalTankLevel = 0;

    /**
     * Pointers to sensors
     */
    Sensor* collectorAverageTemperatureSensor = nullptr;
    Sensor* tankTemperatureSensor = nullptr;
    Sensor* tankLevelSensor = nullptr;

    /**
     * Pointers to actuators
     */
    Actuator* collectorValve = nullptr;
    Actuator* drainValve = nullptr;
    Actuator* tankValve = nullptr;
    Actuator* tankDrainValve = nullptr;

    /**
     * Pointer to process settings
     */
    std::unordered_map<const char*, Setting*>* settings = nullptr;

    /**
     * Goal temperature is reached in collector (with correction)
     *
     * @return Goal temperature is reached
     */
    bool collectorIsHot();

    /**
     * Tank is empty
     *
     * @return Tank is empty
     */
    bool tankIsEmpty();

    /**
     * Tank is full (has reached goal level)
     *
     * @return Tank is full
     */
    bool tankReachedGoal();
    
    /**
     * Get tank level of loads
     *
     * Values if full tank has 4 loads:
     * 0-15%: 0
     * 16-40%: 1
     * 40-65%: 2
     * 65-90%: 3
     * 90-100%: 4
     *
     * @return Load level of the tank
     */
    unsigned char getTankLoads();

    /**
     * Calculate cooling correction
     *
     * @return Temperature (in Celsius) to correct cooling of the tank
     */
    unsigned int calculateCoolingCorrection();

    /**
     * Wait (deactivate all valves)
     */
    void wait();

    /**
     * Drain the pipe
     **/
    void drainPipe();

    /**
     * Collect the water from the collector to the tank
     */
    void collect();

    /**
     * Drain the tank
     *
     * @param partial Partial draining (above 75%)
     */
    void drainTank(bool partial = false);
};

#endif // SOWACO_PROCESS_H
