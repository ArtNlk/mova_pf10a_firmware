#ifndef MAINTASKEVENTS_H
#define MAINTASKEVENTS_H

#include <cstdint>

enum MainTaskEventType : uint8_t {
    MAIN_NULL_EVENT,
    MAIN_BUTTON_PRESS_SHORT,
    MAIN_BUTTON_PRESS_LONG,
    MAIN_BUTTON_PRESS_VERYLONG,
    MAIN_POWER_RESTORED,
    MAIN_POWER_LOST,
    BATTERY_POWER_LOW
};

struct MainTaskEvent {
    MainTaskEventType eventType;
    union
    {

    };
};

#endif