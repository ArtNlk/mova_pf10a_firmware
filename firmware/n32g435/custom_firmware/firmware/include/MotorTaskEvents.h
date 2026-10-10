#ifndef MOTORTASKEVENTS_H
#define MOTORTASKEVENTS_H

#include <cstdint>

enum MotorTaskEventType : uint8_t {
    MOTOR_NULL_EVENT,
    MOTOR_DISPENSE_N
};

struct MotorDispenseEventData {
    uint8_t count;
};

struct MotorTaskEvent {
    MotorTaskEventType eventType;
    union
    {
        MotorDispenseEventData dispenseEventData;
    };
};

#endif