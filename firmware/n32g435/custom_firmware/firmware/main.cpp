#ifdef __cplusplus
extern "C" {
#endif

#include "n32g43x.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
}
#endif

struct LEDParameters {
    uint32_t flashRate;
    GPIO_Module* ledPort;
    uint16_t ledPin;
};

//Led1-PB10
#define LED1_PORT   GPIOB
#define LED1_PIN    GPIO_PIN_10

//Led2-PB11
#define LED2_PORT   GPIOB
#define LED2_PIN    GPIO_PIN_11

#define STACK_SIZE 200
static StaticTask_t led1TaskBuffer;
static StackType_t led1TaskStack[ STACK_SIZE ];
static LEDParameters led1TaskParameters{portTICK_PERIOD_MS*1000,LED1_PORT, LED1_PIN};

static StaticTask_t led2TaskBuffer;
static StackType_t led2TaskStack[ STACK_SIZE ];
static LEDParameters led2TaskParameters{portTICK_PERIOD_MS*2000,LED2_PORT, LED2_PIN};

void LedInit(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIO_InitType GPIO_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    /* Enable the GPIO Clock */
    if (GPIOx == GPIOA)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    }
    else if (GPIOx == GPIOC)
    {
        RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE);
    }
    else
    {
        if (GPIOx == GPIOD)
        {
            RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, ENABLE);
        }
    }

    /* Configure the GPIO pin */
    if (Pin <= GPIO_PIN_ALL)
    {
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin        = Pin;
        GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
        GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_InitPeripheral(GPIOx, &GPIO_InitStructure);
    }
}

void LedOn(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIOx->PBSC = Pin;
}

void LedOff(GPIO_Module* GPIOx, uint16_t Pin)
{
    GPIOx->PBC = Pin;
}

static void ledFlashTask( void * pvParameters )
{
    LEDParameters * ledParams;

    /* Queue a message for printing to say the task has started. */
    // vPrintDisplayMessage( &pcTaskStartMsg );

    ledParams = static_cast<LEDParameters*>(pvParameters);

    LedOn( ledParams->ledPort, ledParams->ledPin );

    for( ; ; )
    {
        /* Delay for half the flash period then turn the LED on. */
        vTaskDelay( ledParams->flashRate);
        LedOn( ledParams->ledPort, ledParams->ledPin );

        // /* Delay for half the flash period then turn the LED off. */
        vTaskDelay( ledParams->flashRate);
        LedOff( ledParams->ledPort, ledParams->ledPin );
    }
}

void setupHardware()
{
    LedInit(GPIOB, LED1_PIN);
    LedInit(GPIOB, LED2_PIN);
}

void startTasks()
{
    xTaskCreateStatic(
        &ledFlashTask,
        "TLED1",
        STACK_SIZE,
        &led1TaskParameters,
        1,
        led1TaskStack,
        &led1TaskBuffer);

    xTaskCreateStatic(
        &ledFlashTask,
        "TLED2",
        STACK_SIZE,
        &led2TaskParameters,
        1,
        led2TaskStack,
        &led2TaskBuffer);
}

int main()
{
    setupHardware();

    startTasks();

    vTaskStartScheduler();

    while(true)
    {

    }

    return 0;
}