#ifndef UARTLOGGER_H
#define UARTLOGGER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "n32g43x.h"
#ifdef __cplusplus
}
#endif

#include <cstdint>
#include <charconv>

#include "RingBuffer.h"

#define log() UartLogger::i() << "\r\n"

class UartLogger
{
public:
    static const size_t MaxStringLength = 128;

    static DMA_ChannelType* UART4_Tx_DMA_Channel; 
    static GPIO_Module* UART4_GPIO;

    static UartLogger& i();

    void onDMADone();
    void tryFlushLogs();

    UartLogger& operator<<(char character);
    UartLogger& operator<<(const char* string);

    template<class T>
    UartLogger& operator<<(T value)
    {
        logValue(value);
        return *this;
    }

    void logChar(char character);
    void logText(const char* string);

    template<class T>
    void logValue(T value)
    {
        char temp[32];
        std::to_chars_result result = std::to_chars(temp, temp + sizeof(temp),value);
        if (result.ec != std::errc())
            logText("Invalid");
        else
        {
            m_sendBuffer.put(temp, result.ptr - temp);
        }
    }

protected:
    void setupUart();

    void startDMA();

    bool dmaTransferInProgress();

    UartLogger();
    ~UartLogger() = default;

    RingBuffer<char, 2048> m_sendBuffer;
    size_t m_lastDmaTransferSize;
    DMA_InitType m_DMAParamStructure;
};

#endif