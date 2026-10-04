#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "semphr.h"
#ifdef __cplusplus
}
#endif

class RecursiveMutexLock
{
public:

    RecursiveMutexLock(SemaphoreHandle_t mutex);
    ~RecursiveMutexLock();

protected:
    SemaphoreHandle_t m_lockedMutex;
};

#endif