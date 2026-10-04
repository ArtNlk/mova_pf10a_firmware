#ifndef POWERMANAGERTASK_H
#define POWERMANAGERTASK_H

#include "n32g43x.h"

#include "StaticTask.h"

class PowerManagerTask : StaticTask<256>
{
public:
    PowerManagerTask();
    ~PowerManagerTask() = default;

    void initHardware();

    static void PowerManagerTaskMain(void* taskParam);

protected:
    void toggleWifiModule(bool isEnabled);

    GPIO_Module* m_mainPortD = GPIOD;

    static const uint16_t WifiEnalbePin = GPIO_PIN_14;
};

#endif