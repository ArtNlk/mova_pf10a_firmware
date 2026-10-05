add_library(FreeRTOS
    ./src/tasks.c
    ./src/port.c
    ./src/list.c
    ./src/queue.c
)

target_compile_options(FreeRTOS PRIVATE -Os -flto -g)
target_include_directories(FreeRTOS PUBLIC ./include)
target_link_libraries(FreeRTOS PUBLIC n32g43x_bsp)