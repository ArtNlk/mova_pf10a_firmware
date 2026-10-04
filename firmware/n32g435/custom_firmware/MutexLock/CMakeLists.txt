add_library(MutexLock
            ${CMAKE_CURRENT_SOURCE_DIR}/src/RecursiveLock.cpp
)

target_compile_options(MutexLock PRIVATE -Os -flto)
target_include_directories(MutexLock PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(MutexLock FreeRTOS)