#ifndef POWERMANAGERTASK_H
#define POWERMANAGERTASK_H

#include "n32g43x.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "StaticTask.h"
#include "PMTaskEvents.h"

class PowerManagerTask : StaticTask<256, 32, PMTaskEvent>
{
public:
    PowerManagerTask(QueueHandle_t* outEventQueue);
    ~PowerManagerTask() = default;

    void initHardware();

    static void PowerManagerTaskMain(void* taskParam);

protected:
    void toggleWifiModule(bool isEnabled);

    GPIO_Module* m_mainPortD = GPIOD;

    static const uint16_t WifiEnalbePin = GPIO_PIN_14;
};

#endif