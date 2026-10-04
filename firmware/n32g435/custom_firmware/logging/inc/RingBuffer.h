#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "FreeRTOS.h"
#include "semphr.h"
#ifdef __cplusplus
}
#endif

#include <cstddef>
#include <array>
#include <algorithm>

#include "RecursiveLock.h"

template<class BufferType, size_t RingBufferCapacity>
class RingBuffer
{
public:
    RingBuffer() :
        m_buffer(),
        m_head(0),
        m_tail(0),
        m_isFull(false)
    {
        m_semaphore = xSemaphoreCreateRecursiveMutexStatic(&m_semaphoreBuf);
    }

    ~RingBuffer() = default;

    bool isFull() const 
    {
        RecursiveMutexLock lock(m_semaphore);
        return m_isFull;   
    }

    bool isEmpty() const
    {
        RecursiveMutexLock lock(m_semaphore);
        return !m_isFull && (m_head == m_tail);
    }

    size_t capacity() const
    {
        return RingBufferCapacity;
    }

    size_t size() const
    {
        RecursiveMutexLock lock(m_semaphore);
        if(m_isFull)
        {
            return RingBufferCapacity;
        }
        
        if(m_head >= m_tail)
        {
            return m_head - m_tail;
        }
        else
        {
            return RingBufferCapacity + m_head - m_tail;
        }
    }

    size_t freeSize() const
    {
        RecursiveMutexLock lock(m_semaphore);
        return RingBufferCapacity - size();
    }

    void put(const BufferType& item)
    {
        RecursiveMutexLock lock(m_semaphore);
        m_buffer[m_head] = item;

        if(m_isFull)
        {
            m_tail = (m_tail + 1) % RingBufferCapacity;
        }

        m_head = (m_head + 1) % RingBufferCapacity;

        m_isFull = m_head == m_tail;
    }

    void put(const BufferType* item, size_t count)
    {
        RecursiveMutexLock lock(m_semaphore);
        if (count > RingBufferCapacity)
        {
            item += count - RingBufferCapacity;
            count = RingBufferCapacity;
        }

        const bool willFill = count > freeSize();

        const size_t firstRangeSize = std::min(count, RingBufferCapacity - m_head);

        std::copy_n(item, firstRangeSize, &m_buffer[m_head]);

        const size_t secondRangeSize = count - firstRangeSize;
        if (secondRangeSize > 0)
        {
            std::copy_n(item + firstRangeSize, secondRangeSize, &m_buffer[0]);
        }

        m_head = (m_head + count) % RingBufferCapacity;

        if (willFill)
        {
            m_tail = m_head;
        }

        m_isFull = m_head == m_tail;
    }

    BufferType get()
    {
        RecursiveMutexLock lock(m_semaphore);
        if(isEmpty())
        {
            return BufferType();
        }

        auto val = m_buffer[m_tail];
        m_isFull = false;
        m_tail = (m_tail + 1) % RingBufferCapacity;

        return val;
    }

    void consume(size_t count)
    {
        RecursiveMutexLock lock(m_semaphore);

        if(isEmpty())
        {
            return;
        }

        const size_t toConsume = std::min(size(), count);

        m_isFull = false;
        m_tail = (m_tail + toConsume) % RingBufferCapacity;
    }

    BufferType* tailPtr()
    {
        RecursiveMutexLock lock(m_semaphore);
        return &m_buffer[m_tail];
    }

    size_t contiguousSize() const
    {
        RecursiveMutexLock lock(m_semaphore);
        
        if (m_isFull)
        {
            return RingBufferCapacity;
        }

        if(m_head >= m_tail)
        {
            return m_head - m_tail;
        }
        else
        {
            return RingBufferCapacity - m_tail;
        }
    }

protected:
    std::array<BufferType, RingBufferCapacity> m_buffer;
    size_t m_head;
    size_t m_tail;
    bool m_isFull;

    SemaphoreHandle_t m_semaphore;
    StaticSemaphore_t m_semaphoreBuf;
};

#endif
