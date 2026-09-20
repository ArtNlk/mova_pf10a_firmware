add_executable(Test.elf main.cpp)

target_compile_options(Test.elf PRIVATE -Os -flto)
target_link_options(Test.elf PRIVATE "-Wl,-Map=${CMAKE_BINARY_DIR}/Test.map")
target_link_libraries(Test.elf n32g43x_bsp)

add_custom_target(Test.bin ALL DEPENDS Test.elf)
add_custom_command(TARGET Test.bin
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary Test.elf Test.bin)