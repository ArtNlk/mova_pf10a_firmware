#include "MainTask.h"

#include "UartLogger.h"

MainTask::MainTask(QueueHandle_t *outEventQueue) :
    StaticTask(&MainTaskMain, "MainTask", this, 1, outEventQueue)
{
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
    }
}
