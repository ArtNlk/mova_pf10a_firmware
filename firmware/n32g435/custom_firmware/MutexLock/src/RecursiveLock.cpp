#include "RecursiveLock.h"

RecursiveMutexLock::RecursiveMutexLock(SemaphoreHandle_t mutex)
{
    xSemaphoreTakeRecursive(mutex, portTICK_PERIOD_MS*100);

    m_lockedMutex = mutex;
}

RecursiveMutexLock::~RecursiveMutexLock()
{
    xSemaphoreGiveRecursive(m_lockedMutex);
}
