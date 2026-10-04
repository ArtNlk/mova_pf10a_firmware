#ifndef UITASK_H
#define UITASK_H

#include "StaticTask.h"

#include "n32g43x.h"

class UITask : public StaticTask<256>
{
public:
    UITask();
    ~UITask() = default;

    static const uint16_t MainButtonInterruptPin = EXTI_LINE13;

    static const uint16_t RedLedPin = GPIO_PIN_10;
    static const uint16_t GreenLedPin = GPIO_PIN_11;
    static const uint16_t BlueLedPin = GPIO_PIN_12;
    static const uint16_t MainButtonPin = GPIO_PIN_13;
    static const uint16_t BottomButtonPin = GPIO_PIN_13;

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

    GPIO_Module* m_mainPortB = GPIOB;
    GPIO_Module* m_bottomButtonPort = GPIOC;
};

#endif