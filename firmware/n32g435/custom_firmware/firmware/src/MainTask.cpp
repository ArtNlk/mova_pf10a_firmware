#include "MainTask.h"

#include "UartLogger.h"

MainTask::MainTask(QueueHandle_t *outEventQueue) :
    StaticTask(&MainTaskMain, "MainTask", this, 1, outEventQueue),
    m_feedMotor()
{
}

void MainTask::onMotorTickInterrupt()
{
    m_feedMotor.onMotorTickInterrupt();
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
                task->m_feedMotor.dispensePortions(5);
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
