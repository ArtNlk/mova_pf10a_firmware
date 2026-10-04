#ifndef STATICTASK_H
#define STATICTASK_H

#include "FreeRTOS.h"
#include "task.h"

template<uint32_t stackDepth>
class StaticTask {
    public:

    StaticTask(TaskFunction_t functionCode, const char* name, void * const taskParameters, UBaseType_t taskPriority)
    {
        xTaskCreateStatic(
            functionCode,
            name,
            stackDepth,
            taskParameters,
            taskPriority,
            m_taskStack,
            &m_taskStorage);
    }
    ~StaticTask() = default;

protected:
    StaticTask_t m_taskStorage;
    StackType_t m_taskStack[stackDepth];
};

#endif