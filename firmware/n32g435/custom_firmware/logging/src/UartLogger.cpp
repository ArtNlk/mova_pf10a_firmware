#include "UartLogger.h"

DMA_ChannelType* UartLogger::UART4_Tx_DMA_Channel = DMA_CH1;
GPIO_Module* UartLogger::UART4_GPIO = GPIOB;

UartLogger &UartLogger::i()
{
    static UartLogger logger;
    return logger;
}

void UartLogger::onDMADone()
{
    DMA_EnableChannel(UART4_Tx_DMA_Channel, DISABLE);
}

void UartLogger::tryFlushLogs()
{
    if(dmaTransferInProgress())
    {
        return;
    }
    else if(m_lastDmaTransferSize > 0)
    {
        m_sendBuffer.consume(m_lastDmaTransferSize);
        m_lastDmaTransferSize = 0;
    }

    startDMA();
}

UartLogger &UartLogger::operator<<(char character)
{
    logChar(character);
    return *this;
}

UartLogger &UartLogger::operator<<(const char *inputString)
{
    logText(inputString);
    return *this;
}

void UartLogger::logChar(char character)
{
    m_sendBuffer.put(character);
}

void UartLogger::logText(const char *inputString)
{
    const char* end = std::find(inputString, inputString + MaxStringLength, '\0');
    const size_t stringLength = static_cast<size_t>(end - inputString);
    m_sendBuffer.put(inputString, stringLength);
}

void UartLogger::setupUart()
{
    /* DMA clock enable */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);
    /* Enable GPIO clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    /* Enable USART4 Clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_UART4, ENABLE);

    NVIC_InitType NVIC_InitStructure;

    /* Enable DMA1 channel1 IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel                   = DMA_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_InitType GPIO_InitStructure;

    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);

    /* Configure USART4 Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = GPIO_PIN_0; 
    GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;	
    GPIO_InitStructure.GPIO_Mode      = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_UART4;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

    /* Configure USART4 Rx as alternate function push-pull and pull-up */
    GPIO_InitStructure.Pin            = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_UART4;
    GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure); 

    /* USART4_Tx_DMA_Channel (triggered by USART4 Tx event) Config */
    DMA_DeInit(UART4_Tx_DMA_Channel);
    DMA_StructInit(&m_DMAParamStructure);
    m_DMAParamStructure.PeriphAddr     = reinterpret_cast<uint32_t>(&UART4->DAT);
    m_DMAParamStructure.MemAddr        = reinterpret_cast<uint32_t>(m_sendBuffer.tailPtr());
    m_DMAParamStructure.Direction      = DMA_DIR_PERIPH_DST;
    m_DMAParamStructure.BufSize        = m_sendBuffer.size();
    m_DMAParamStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
    m_DMAParamStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
    m_DMAParamStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
    m_DMAParamStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
    m_DMAParamStructure.CircularMode   = DMA_MODE_NORMAL;
    m_DMAParamStructure.Priority       = DMA_PRIORITY_MEDIUM;
    m_DMAParamStructure.Mem2Mem        = DMA_M2M_DISABLE;
    DMA_Init(UART4_Tx_DMA_Channel, &m_DMAParamStructure);
    DMA_RequestRemap(DMA_REMAP_UART4_TX, DMA, UART4_Tx_DMA_Channel, ENABLE);
    DMA_ConfigInt(UART4_Tx_DMA_Channel, DMA_INT_TXC, ENABLE);

    USART_InitType USART_InitStructure;
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.BaudRate            = 115200;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode                = USART_MODE_TX; //Tx-only for now
    USART_Init(UART4, &USART_InitStructure);

    USART_EnableDMA(UART4, USART_DMAREQ_TX, ENABLE); //Tx-only for now
    USART_Enable(UART4, ENABLE);
}

void UartLogger::startDMA()
{
    const size_t transferSize = m_sendBuffer.contiguousSize();
    if(transferSize == 0)
    {
        return;
    }

    m_DMAParamStructure.MemAddr = reinterpret_cast<uint32_t>(m_sendBuffer.tailPtr());
    m_DMAParamStructure.BufSize = transferSize;
    DMA_EnableChannel(UART4_Tx_DMA_Channel, DISABLE);
    DMA_DeInit(UART4_Tx_DMA_Channel);
    DMA_ClrIntPendingBit(DMA_INT_GLB1 | DMA_INT_TXC1 | DMA_INT_HTX1 | DMA_INT_ERR1, DMA);
    DMA_Init(UART4_Tx_DMA_Channel, &m_DMAParamStructure);
    m_lastDmaTransferSize = transferSize;
    DMA_RequestRemap(DMA_REMAP_UART4_TX, DMA, UART4_Tx_DMA_Channel, ENABLE);
    DMA_ConfigInt(UART4_Tx_DMA_Channel, DMA_INT_TXC, ENABLE);

    DMA_EnableChannel(UART4_Tx_DMA_Channel, ENABLE);
}

bool UartLogger::dmaTransferInProgress()
{
    return DMA_GetCurrDataCounter(UartLogger::UART4_Tx_DMA_Channel) != 0;
}

UartLogger::UartLogger() :
    m_lastDmaTransferSize(0)
{
    setupUart();
}
