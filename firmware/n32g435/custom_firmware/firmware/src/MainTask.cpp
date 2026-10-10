#include "MainTask.h"

#include "UartLogger.h"
#include "MotorTaskEvents.h"

MainTask::MainTask(QueueHandle_t *outEventQueue) :
    StaticTask(&MainTaskMain, "MainTask", this, 1, outEventQueue),
    m_motorTaskQueue(nullptr)
{
}

void MainTask::setMotorTaskQueue(QueueHandle_t motorTaskQueue)
{
    m_motorTaskQueue = motorTaskQueue;
}

void MainTask::MainTaskMain(void *taskParam)
{
    log() << "MainTask started";
    MainTask* task = static_cast<MainTask*>(taskParam);
    MainTaskEvent event = MainTaskEvent();
    while(true)
    {
        if(xQueueReceive(task->eventQueue(), &event, portTICK_PERIOD_MS*250) == errQUEUE_EMPTY)
        {
            continue;
        }

        log() << "MAIN got event " << event.eventType;

        switch (event.eventType)
        {
            case MainTaskEventType::MAIN_BUTTON_PRESS_SHORT:
            {
                MotorTaskEvent motorEvent;
                motorEvent.eventType = MotorTaskEventType::MOTOR_DISPENSE_N;
                motorEvent.dispenseEventData.count = 5;
                xQueueSendToBack(task->m_motorTaskQueue, &motorEvent, pdMS_TO_TICKS(10));
                break;
            }
            case MainTaskEventType::MAIN_BUTTON_PRESS_LONG:
            {
                break;
            }
            case MainTaskEventType::MAIN_BUTTON_PRESS_VERYLONG:
            {
                break;
            }
        
        default:
            break;
        }
    }
}
