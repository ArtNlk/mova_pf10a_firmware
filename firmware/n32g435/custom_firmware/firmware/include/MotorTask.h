#ifndef MOTORTASK_H
#define MOTORTASK_H

#include "n32g43x.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "StaticTask.h"
#include "MotorTaskEvents.h"

class MotorTask : StaticTask<256, 32, MotorTaskEvent>
{
public:
    MotorTask(QueueHandle_t* outEventQueue);
    ~MotorTask() = default;

    static const uint32_t SpinCtlReadEXTILine = EXTI_LINE9;

    void onPortionDispensed();

protected:
    static void MotorTaskMain(void* taskParam);

    static GPIO_Module* GPIOPortA;
    static const uint16_t MotorForwardPin = GPIO_PIN_0;
    static const uint16_t MotorBackwardPin = GPIO_PIN_1;
    static const uint16_t SpinCtlEnablePin = GPIO_PIN_15;

    static const uint16_t SpinCtlReadPin = GPIO_PIN_9;
    static const uint8_t  SpinCtlReadEXTISource = GPIO_PIN_SOURCE9;

    enum MotorState : uint16_t
    {
        MOTOR_STOP = 0,
        MOTOR_FORWARD = MotorForwardPin,
        MOTOR_BACKWARD = MotorBackwardPin,
        MOTOR_BRAKE = MotorForwardPin | MotorBackwardPin
    };

    void setMotorState(MotorState newState);

    void spinCtlToggle(bool isEnabled);

    void setupSpinCtlInterrupt();
    void initMotorPins();

    uint8_t m_remainingPortionCount;
};

#endif