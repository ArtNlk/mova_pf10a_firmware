#ifndef MAINTASK_H
#define MAINTASK_H

#include <array>

#include "FreeRTOS.h"
#include "queue.h"

#include "MainTaskEvents.h"
#include "StaticTask.h"

class MainTask : StaticTask<256, 32, MainTaskEvent>
{
public:
    MainTask(QueueHandle_t* outEventQueue);
    ~MainTask() = default;

    void setMotorTaskQueue(QueueHandle_t motorTaskQueue);

protected:
    static void MainTaskMain(void* taskParam);

    QueueHandle_t m_motorTaskQueue;
};

#endif