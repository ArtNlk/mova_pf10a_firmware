add_executable(N32G435_fw.elf 
    src/StaticTask.cpp
    src/MainTask.cpp
    src/UITask.cpp
    src/PowerManagerTask.cpp
    main.cpp
    )

target_include_directories(N32G435_fw.elf PUBLIC ./include)

target_compile_options(N32G435_fw.elf PRIVATE -Os -flto -g)
target_link_options(N32G435_fw.elf PRIVATE "-Wl,-Map=${CMAKE_BINARY_DIR}/N32G435_fw.map")
target_link_libraries(N32G435_fw.elf n32g43x_bsp)
target_link_libraries(N32G435_fw.elf FreeRTOS)
target_link_libraries(N32G435_fw.elf LoggingLib)


add_custom_target(N32G435_fw.bin ALL DEPENDS N32G435_fw.elf)
add_custom_command(TARGET N32G435_fw.bin
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary N32G435_fw.elf N32G435_fw.bin)