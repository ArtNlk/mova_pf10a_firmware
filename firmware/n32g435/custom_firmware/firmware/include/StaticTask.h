#ifndef STATICTASK_H
#define STATICTASK_H

#include <array>

#include "FreeRTOS.h"
#include "task.h"

template<uint32_t stackDepth, size_t QueueSize, class TaskEventType>
class StaticTask {
    public:

    StaticTask(TaskFunction_t functionCode, const char* name, void * const taskParameters, UBaseType_t taskPriority, QueueHandle_t* outEventQueue)
    {
        xTaskCreateStatic(
            functionCode,
            name,
            stackDepth,
            taskParameters,
            taskPriority,
            m_taskStack,
            &m_taskStorage);
        m_eventQueue = xQueueCreateStatic(QueueSize,sizeof(TaskEventType),m_eventQueueStorage.data(), &m_eventQueueBuffer);
        *outEventQueue = m_eventQueue;
    }
    ~StaticTask() = default;

    QueueHandle_t eventQueue() const
    {
        return m_eventQueue;
    }

protected:
    StaticTask_t m_taskStorage;
    StackType_t m_taskStack[stackDepth];

    QueueHandle_t m_eventQueue;
    StaticQueue_t m_eventQueueBuffer;
    std::array<uint8_t,QueueSize*sizeof(TaskEventType)> m_eventQueueStorage;
};

#endif