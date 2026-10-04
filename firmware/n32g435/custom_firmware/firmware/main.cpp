#ifdef __cplusplus
extern "C" {
#endif
#include "n32g43x.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
}
#endif

#include "UITask.h"
#include "PowerManagerTask.h"
#include "UartLogger.h"

void startTasks()
{
    static UITask uiTask;
    static PowerManagerTask pmTask;
}

int main()
{   
    log() << "Main started";

    startTasks();

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

#ifdef __cplusplus
}
#endif
