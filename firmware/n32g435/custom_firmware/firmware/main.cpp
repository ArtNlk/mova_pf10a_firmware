#ifdef __cplusplus
extern "C" {
#endif
#include "n32g43x.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
}
#endif

#include "MainTask.h"
#include "UITask.h"
#include "PowerManagerTask.h"
#include "UartLogger.h"

#include "FeedMotor.h"

static QueueHandle_t UITaskEventQueue = nullptr;
static QueueHandle_t PMTaskEventQueue = nullptr;
static QueueHandle_t MainTaskEventQueue = nullptr;

static MainTask* MainTaskPtr = nullptr;

void initTasks()
{
    static MainTask mainTask(&MainTaskEventQueue);
    static UITask uiTask(&UITaskEventQueue);
    static PowerManagerTask pmTask(&PMTaskEventQueue);

    MainTaskPtr = &mainTask;

    uiTask.setMainTaskEventQueue(MainTaskEventQueue);
}

int main()
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    log() << "Main started";

    initTasks();

    vTaskStartScheduler();

    while(true)
    {

    }

    return 0;
}

//FreeRTOS idle hook
void vApplicationIdleHook( void )
{
    UartLogger::i().tryFlushLogs();
}

#ifdef __cplusplus
extern "C" {
#endif

//Logger DMA IRQ
void DMA_Channel1_IRQHandler(void)
{
    /* Test on DMA channel Transfer Complete interrupt */
    if (DMA_GetIntStatus(DMA_INT_TXC1, DMA))
    {

        /* Get Current Data Counter value after complete transfer */
        if(DMA_GetCurrDataCounter(UartLogger::UART4_Tx_DMA_Channel) == 0)
        {
            UartLogger::i().onDMADone();
        }
    }

    /* Clear DMA channel Half Transfer, Transfer Complete and Global interrupt pending bits */
    DMA_ClrIntPendingBit(DMA_INT_GLB1 | DMA_INT_TXC1 | DMA_INT_HTX1 | DMA_INT_ERR1, DMA);
}

//Spin control interrupt
void EXTI9_5_IRQHandler(void)
{
    if (RESET != EXTI_GetStatusFlag(FeedMotor::SpinCtlReadEXTILine))
    {
        if(MainTaskPtr != nullptr)
        {
            MainTaskPtr->onMotorTickInterrupt();
        }
        EXTI_ClrITPendBit(FeedMotor::SpinCtlReadEXTILine);
    }
}

//Main button press/release IRQ
void EXTI15_10_IRQHandler(void)
{
    BaseType_t yieldRequired = pdFALSE;

    if (RESET != EXTI_GetStatusFlag(UITask::MainButtonInterruptLine))
    {
        EXTI_ClrITPendBit(UITask::MainButtonInterruptLine);
        if(UITaskEventQueue != nullptr)
        {
            UITaskEvent temp = UITaskEvent();
            TickType_t eventTick = xTaskGetTickCountFromISR();
            if(GPIO_ReadInputDataBit(UITask::MainLedButtonPortB, UITask::MainButtonPin) == Bit_RESET)
            {
                temp.eventType = UITaskEventType::MAIN_BUTTON_PRESSED;
                temp.pressEventData.pressTick = eventTick;
                xQueueSendFromISR(UITaskEventQueue, &temp, &yieldRequired);
            }
            else
            {
                temp.eventType = UITaskEventType::MAIN_BUTTON_RELEASED;
                temp.releaseEventData.releaseTick = eventTick;
                xQueueSendFromISR(UITaskEventQueue, &temp, &yieldRequired);
            }
        }
    }

    portYIELD_FROM_ISR(yieldRequired);
}

#ifdef __cplusplus
}
#endif
