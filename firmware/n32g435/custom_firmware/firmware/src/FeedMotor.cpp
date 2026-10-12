#include "FeedMotor.h"

#include "UartLogger.h"

GPIO_Module* FeedMotor::GPIOPortA = GPIOA;

FeedMotor::FeedMotor() :
    m_remainingPortionsCount(0)
{
    initMotorPins();
    setupSpinCtlInterrupt();
}

void FeedMotor::dispensePortions(uint8_t portionCount)
{
    m_remainingPortionsCount += portionCount + 1;
    setMotorState(MotorState::MOTOR_FORWARD);
}

void FeedMotor::onMotorTickInterrupt()
{
    if(m_remainingPortionsCount != 0)
    {
        m_remainingPortionsCount--;
    }

    if(m_remainingPortionsCount == 0)
    {
        setMotorState(MOTOR_STOP);
    }
}

void FeedMotor::setMotorState(MotorState newState)
{
    spinCtlToggle(newState == MotorState::MOTOR_FORWARD || newState == MotorState::MOTOR_BACKWARD);

    GPIOPortA->PBSC = (newState) | ((newState ^ MotorState::MOTOR_BRAKE) << 16);
}

void FeedMotor::spinCtlToggle(bool isEnabled)
{
    GPIOPortA->PBSC = SpinCtlEnablePin << (isEnabled? 0 : 16);
}

void FeedMotor::setupSpinCtlInterrupt()
{
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

    /*Configure key EXTI Line to key input Pin*/
    GPIO_ConfigEXTILine(GPIOA_PORT_SOURCE, SpinCtlReadEXTISource);

    /*Configure key EXTI line*/
    EXTI_InitStructure.EXTI_Line    = SpinCtlReadEXTILine;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitPeripheral(&EXTI_InitStructure);

    /*Set key input interrupt priority*/
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void FeedMotor::initMotorPins()
{
    GPIO_InitType GPIO_InitStructure;

    /* GPIOA clock enable */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_AFIO, ENABLE);

    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin        = MotorForwardPin | MotorBackwardPin | SpinCtlEnablePin;
    GPIO_InitStructure.GPIO_Current = GPIO_DC_4mA;
    GPIO_InitStructure.GPIO_Pull    = GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOPortA, &GPIO_InitStructure);

    GPIO_InitStructure.Pin          = SpinCtlReadPin;
    GPIO_InitStructure.GPIO_Pull    = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Input;
    GPIO_InitPeripheral(GPIOPortA, &GPIO_InitStructure);
}
