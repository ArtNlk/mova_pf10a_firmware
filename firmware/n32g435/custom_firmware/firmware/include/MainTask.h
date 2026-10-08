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

protected:
    static void MainTaskMain(void* taskParam);

    static const size_t QueueSize = 32;
    QueueHandle_t m_eventQueue;
    StaticQueue_t m_eventQueueBuffer;
    std::array<uint8_t,QueueSize*sizeof(MainTaskEvent)> m_eventQueueStorage;
};

#endif