add_library(LoggingLib
            ${CMAKE_CURRENT_SOURCE_DIR}/src/RingBuffer.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/UartLogger.cpp
)

target_compile_options(LoggingLib PRIVATE -Os -flto)
target_include_directories(LoggingLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(LoggingLib n32g43x_bsp)
target_link_libraries(LoggingLib FreeRTOS)
target_link_libraries(LoggingLib MutexLock)