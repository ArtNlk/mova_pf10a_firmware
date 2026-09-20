set(BSP_DEFAULT_LINKER_FILE ${CMAKE_CURRENT_LIST_DIR}//CMSIS/device/n32g43x_flash.ld CACHE INTERNAL "Default Linker Script" FORCE)

add_library(n32g43x_bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/misc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_can.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_comp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_crc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_dac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_dbg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_iwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_lptim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_lpuart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_opamp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_tsc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/src/n32g43x_wwdg.c

    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/device/system_n32g43x.c

    # ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_usbfs_driver/src/usb_core.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_usbfs_driver/src/usb_init.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_usbfs_driver/src/usb_int.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_usbfs_driver/src/usb_mem.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_usbfs_driver/src/usb_regs.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_usbfs_driver/src/usb_sil.c

    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/device/startup/startup_n32g43x_gcc.s
)

target_compile_options(n32g43x_bsp PRIVATE -Os -flto)

target_include_directories(n32g43x_bsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_std_periph_driver/inc)
# target_include_directories(n32g43x_bsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/n32g43x_usbfs_driver/inc)
target_include_directories(n32g43x_bsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/core)
target_include_directories(n32g43x_bsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/device)