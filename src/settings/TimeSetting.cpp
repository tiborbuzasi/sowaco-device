/**
 * Class for Time Setting Setting
 *
 * @author Tibor Buzási <develop@tiborbuzasi.com>
 * 
 * Copyright © 2020 Tibor Buzási. All rights reserved.
 * For licensing information see LICENSE in the project root folder.
 */

#include "TimeSetting.hpp"

#include <Arduino.h>
#include <sys/time.h>
#include <time.h>

TimeSetting::TimeSetting(BLEService* service, const char* uuid, const char* name, DataType type,
    const char* group, const char* key, uint32_t permissions, uint32_t properties)
    : Setting(service, uuid, name, type, group, key, false, permissions, properties)
{
    unsigned int value = getTime();

    characteristic->setValue(value);

    notificationNeeded = true;
}

unsigned int TimeSetting::getIntValue()
{
    return getTime();
}

void TimeSetting::setValue(unsigned int value)
{
    if (value == 0)
    {
        value = getTime();
    }
    characteristic->setValue(value);

    notificationNeeded = true;
}

void TimeSetting::onWrite(BLECharacteristic* characteristic)
{
    unsigned int value = (unsigned int)characteristic->getData();
    setTime(value);

#ifdef DEBUG
    Serial.print("System time changed: ");
    Serial.println(value);
#endif

}

time_t TimeSetting::getTime()
{
    time_t now;

    time(&now);

    return now;
}

// Based on https://www.esp32.com/viewtopic.php?t=6043 as at 30.11.2020.
void TimeSetting::setTime(time_t)
{
/*    struct tm tm;
    tm.tm_year = 119;
    tm.tm_mon = 12;
    tm.tm_mday = 12;
    tm.tm_hour = 21;
    tm.tm_min = 20;
    tm.tm_sec = 19;
    time_t time = mktime(&tm);

    timeval now = { .tv_sec = time };
    settimeofday(&now, NULL);*/
}