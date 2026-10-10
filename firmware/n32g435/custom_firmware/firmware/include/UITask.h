#ifndef UITASK_H
#define UITASK_H

#include <array>
#include <limits>

#include "FreeRTOS.h"
#include "queue.h"

#include "StaticTask.h"
#include "UITaskEvents.h"

#include "n32g43x.h"

class UITask : public StaticTask<256, 32, UITaskEvent>
{
public:
    static const TickType_t InvalidMainButtonPressTick = std::numeric_limits<TickType_t>::max();

    UITask(QueueHandle_t* outEventQueue);
    ~UITask() = default;

    void setMainTaskEventQueue(QueueHandle_t mainTaskEventQueue);

    static const uint16_t MainButtonInterruptLine = EXTI_LINE13;

    static const uint16_t RedLedPin = GPIO_PIN_10;
    static const uint16_t GreenLedPin = GPIO_PIN_11;
    static const uint16_t BlueLedPin = GPIO_PIN_12;
    static const uint16_t MainButtonPin = GPIO_PIN_13;
    //static const uint16_t BottomButtonPin = GPIO_PIN_13;

    static GPIO_Module* MainLedButtonPortB;
    static GPIO_Module* BottomButtonPortC;

    enum ButtonColor : uint16_t {
        BLACK = 0,
        RED = RedLedPin,
        GREEN = GreenLedPin,
        BLUE = BlueLedPin,
        YELLOW = RED|GREEN,
        MAGENTA = RED|BLUE,
        CYAN = GREEN|BLUE,
        WHITE = RED|GREEN|BLUE
    };

protected:
    static const TickType_t MinButtonPressDuration = pdMS_TO_TICKS(10);
    static const TickType_t MaxShortButtonPressDuration = pdMS_TO_TICKS(1000); //1 second
    static const TickType_t MaxLongButtonPressDuration = pdMS_TO_TICKS(3000); //3 seconds

    void initLedPins();

    void initButtonPins();

    void setButtonColor(ButtonColor color);

    static void UITaskMain(void* taskParam);

    TickType_t m_mainButtonPressStartTick;
    QueueHandle_t m_mainTaskEventQueue;
};

#endif