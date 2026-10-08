#ifndef UITASKEVENTS_H
#define UITASKEVENTS_H

#include "FreeRTOS.h"

enum UITaskEventType : uint8_t {
    NULL_EVENT,
    MAIN_BUTTON_PRESSED,
    MAIN_BUTTON_RELEASED,
    SET_UI_PATTERN
};

enum UITaskPattern : uint8_t {
    PATTERN_NONE,
    PATTERN_WARNING,
    PATTERN_ERROR,
    PATTERN_OFF
};

struct ButtonPressEventData
{
    TickType_t pressTick;
};

struct ButtonReleaseEventData
{
    TickType_t releaseTick;
};

struct UIPatternEventData
{
    UITaskPattern requestedPattern;
};

struct UITaskEvent {
    UITaskEventType eventType;
    union
    {
        ButtonPressEventData pressEventData;
        ButtonReleaseEventData releaseEventData;
        UIPatternEventData patternRequestData;
    };
};

#endif