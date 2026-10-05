#include "UITask.h"
#include "portmacro.h"

#include "UartLogger.h"

GPIO_Module* UITask::MainLedButtonPortB = GPIOB;
GPIO_Module* UITask::BottomButtonPortC = GPIOC;

void UITask::UITaskMain(void* taskParam)
{
    log() << "UITask started";
    UITask* task = static_cast<UITask*>(taskParam);
    Event event = NULL_EVENT;
    while(true)
    {
        if(xQueueReceive(task->eventQueue(), &event, portTICK_PERIOD_MS*250) == errQUEUE_EMPTY)
        {
            task->setButtonColor(BLUE);
            continue;
        }

        log() << "Got event " << event;

        switch(event)
        {
            case MAIN_BUTTON_PRESSED:
                task->setButtonColor(GREEN);
                break;
            
            case MAIN_BUTTON_RELEASED:
                task->setButtonColor(RED);
                break;
        }
    }
}

UITask::UITask(QueueHandle_t* outEventQueue):
    StaticTask<256>(&UITaskMain, "UITask", this, 1)
{
    m_eventQueue = xQueueCreateStatic(QueueSize,sizeof(Event),m_eventQueueStorage.data(), &m_eventQueueBuffer);
    *outEventQueue = m_eventQueue;
    initLedPins();
    initButtonPins();
}

QueueHandle_t UITask::eventQueue() const
{
    return m_eventQueue;
}

void UITask::initLedPins()
{
    GPIO_InitType GPIO_InitStructure;

    /* GPIOA and GPIOB clock enable */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);

    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = RedLedPin | GreenLedPin | BlueLedPin;
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(MainLedButtonPortB, &GPIO_InitStructure);
}

void UITask::initButtonPins()
{
    GPIO_InitType GPIO_InitStructure;
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /*Configure the GPIO pin as input floating*/
    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = MainButtonPin;
    GPIO_InitStructure.GPIO_Pull    = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
    GPIO_InitPeripheral(MainLedButtonPortB, &GPIO_InitStructure);

    /*Configure key EXTI Line to key input Pin*/
    GPIO_ConfigEXTILine(GPIOB_PORT_SOURCE, GPIO_PIN_SOURCE13);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line    = MainButtonInterruptLine;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void UITask::setButtonColor(ButtonColor color)
{
    log() << "Button color set to " << static_cast<uint32_t>(color);
    MainLedButtonPortB->PBSC = (color) | ((color ^ ButtonColor::WHITE) << 16);
}