#include "PowerManagerTask.h"

PowerManagerTask::PowerManagerTask() :
    StaticTask<256>(&PowerManagerTaskMain, "PMTask", this, 1)
{
    initHardware();
}

void PowerManagerTask::initHardware()
{
    GPIO_InitType GPIO_InitStructure;

    /* GPIOA and GPIOB clock enable */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, ENABLE);

    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = WifiEnalbePin;
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(m_mainPortD, &GPIO_InitStructure);
}

void PowerManagerTask::PowerManagerTaskMain(void *taskParam)
{
    PowerManagerTask* task = static_cast<PowerManagerTask*>(taskParam);

    task->toggleWifiModule(true);

    while(true)
    {
        vTaskDelay(portTICK_PERIOD_MS*1000);
    }
}

void PowerManagerTask::toggleWifiModule(bool isEnabled)
{
    m_mainPortD->PBSC = isEnabled? WifiEnalbePin : WifiEnalbePin << 16;
}