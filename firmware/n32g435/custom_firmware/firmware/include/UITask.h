#ifndef UITASK_H
#define UITASK_H

#include <array>

#include "FreeRTOS.h"
#include "queue.h"

#include "StaticTask.h"

#include "n32g43x.h"

class UITask : public StaticTask<256>
{
public:
    UITask(QueueHandle_t* outEventQueue);
    ~UITask() = default;

    QueueHandle_t eventQueue() const;

    enum Event : uint8_t {
        NULL_EVENT,
        MAIN_BUTTON_PRESSED,
        MAIN_BUTTON_RELEASED
    };

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

    void initLedPins();

    void initButtonPins();

    void setButtonColor(ButtonColor color);

    static void UITaskMain(void* taskParam);

    static const size_t QueueSize = 32;

    QueueHandle_t m_eventQueue;
    StaticQueue_t m_eventQueueBuffer;
    std::array<uint8_t,QueueSize*sizeof(Event)> m_eventQueueStorage;
};

#endif