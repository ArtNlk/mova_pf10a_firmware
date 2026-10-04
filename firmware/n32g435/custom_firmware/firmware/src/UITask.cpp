#include "UITask.h"
#include "portmacro.h"

#include "UartLogger.h"

UITask::ButtonColor globalColor = UITask::ButtonColor::GREEN;

void UITask::UITaskMain(void* taskParam)
{
    UITask* task = static_cast<UITask*>(taskParam);

    while(true)
    {
        task->setButtonColor(ButtonColor::BLACK);
        vTaskDelay(portTICK_PERIOD_MS*1000);
        task->setButtonColor(ButtonColor::RED);
        vTaskDelay(portTICK_PERIOD_MS*1000);
        task->setButtonColor(ButtonColor::GREEN);
        vTaskDelay(portTICK_PERIOD_MS*1000);
        task->setButtonColor(ButtonColor::BLUE);
        vTaskDelay(portTICK_PERIOD_MS*1000);
        task->setButtonColor(ButtonColor::CYAN);
        vTaskDelay(portTICK_PERIOD_MS*1000);
        task->setButtonColor(ButtonColor::MAGENTA);
        vTaskDelay(portTICK_PERIOD_MS*1000);
        task->setButtonColor(ButtonColor::YELLOW);
        vTaskDelay(portTICK_PERIOD_MS*1000);
        task->setButtonColor(ButtonColor::WHITE);
        vTaskDelay(portTICK_PERIOD_MS*1000);
    }
}

UITask::UITask():
    StaticTask<256>(&UITaskMain, "UITask", this, 1)
{
    initLedPins();
}

void UITask::initLedPins()
{
    GPIO_InitType GPIO_InitStructure;

    /* GPIOA and GPIOB clock enable */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);

    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = RedLedPin | GreenLedPin | BlueLedPin;
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(m_mainPortB, &GPIO_InitStructure);
}

void UITask::initButtonPins()
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /*Configure the GPIO pin as input floating*/
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = MainButtonPin;
    GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
    GPIO_InitPeripheral(m_mainPortB, &GPIO_InitStructure);

    /*Configure key EXTI Line to key input Pin*/
    GPIO_ConfigEXTILine(GPIOB_PORT_SOURCE, GPIO_PIN_SOURCE13);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line    = EXTI_LINE13;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void UITask::setButtonColor(ButtonColor color)
{
    log() << "Button color set to " << static_cast<uint32_t>(color);
    m_mainPortB->PBSC = (color) | ((color ^ ButtonColor::WHITE) << 16);
}