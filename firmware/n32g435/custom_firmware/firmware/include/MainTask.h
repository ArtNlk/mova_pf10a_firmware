#ifndef MAINTASK_H
#define MAINTASK_H

#include <array>

#include "FreeRTOS.h"
#include "queue.h"

#include "MainTaskEvents.h"
#include "StaticTask.h"

#include "FeedMotor.h"

class MainTask : StaticTask<256, 32, MainTaskEvent>
{
public:
    MainTask(QueueHandle_t* outEventQueue);
    ~MainTask() = default;

    void onMotorTickInterrupt();

protected:
    static void MainTaskMain(void* taskParam);

    FeedMotor m_feedMotor;
};

#endif