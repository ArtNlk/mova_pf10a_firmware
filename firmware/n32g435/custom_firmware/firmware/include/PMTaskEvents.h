#ifndef PMTASKEVENT_H
#define PMTASKEVENT_H

#include <cstdint>

enum PMTaskEventType : uint8_t {
    PM_NULL_EVENT,
    WIFI_POWER_TOGGLE
};

struct WifiPowerToggleEventData
{
    bool wifiEnabled;
};

struct PMTaskEvent {
    PMTaskEventType eventType;
    union
    {
        WifiPowerToggleEventData wifiToggleEventData;
    };
};

#endif