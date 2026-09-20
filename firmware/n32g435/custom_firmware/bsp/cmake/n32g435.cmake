#------------------------------------------------------------------------------
# CMake toolchain file for Nationstech N32G435 (Cortex-M4F)
# Requires: arm-none-eabi-gcc in PATH (or set TOOLCHAIN_PREFIX)
# Usage:
#   cmake -B build -DCMAKE_TOOLCHAIN_FILE=path/to/n32g435-gcc.cmake ...
#------------------------------------------------------------------------------

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Optional: override the toolchain prefix if not in PATH
# set(TOOLCHAIN_PREFIX "/path/to/gcc-arm-none-eabi/bin/arm-none-eabi-")
if(NOT TOOLCHAIN_PREFIX)
  set(TOOLCHAIN_PREFIX "arm-none-eabi-")
endif()

set(CMAKE_C_COMPILER   ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_OBJCOPY      ${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_OBJDUMP      ${TOOLCHAIN_PREFIX}objdump CACHE INTERNAL "objdump tool")
set(CMAKE_SIZE         ${TOOLCHAIN_PREFIX}size    CACHE INTERNAL "size tool")
set(CMAKE_GDB          ${TOOLCHAIN_PREFIX}gdb     CACHE INTERNAL "debugger")

# Critical for bare-metal: don't try to link a full executable during try_compile
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

#------------------------------------------------------------------------------
# CPU / FPU flags (N32G435 = Cortex-M4F with single-precision FPU)
#------------------------------------------------------------------------------
set(CPU_FLAGS
  -mcpu=cortex-m4
  -mthumb
  -mfloat-abi=hard
  -mfpu=fpv4-sp-d16
  -DARM_MATH_CM4
  -D__FPU_PRESENT=1
)

# Common flags for C / C++ / ASM
set(COMMON_FLAGS
  ${CPU_FLAGS}
  -ffunction-sections
  -fdata-sections
  -fno-common
  -fmessage-length=0
)

string(REPLACE ";" " " COMMON_FLAGS_STR "${COMMON_FLAGS}")

set(CMAKE_C_FLAGS_INIT   "${COMMON_FLAGS_STR}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS_STR}" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS_INIT "${COMMON_FLAGS_STR} -x assembler-with-cpp" CACHE STRING "" FORCE)

# Linker flags (the .ld file itself is supplied by the project)
set(CMAKE_EXE_LINKER_FLAGS_INIT
  "-Wl,-T${CMAKE_CURRENT_LIST_DIR}/../CMSIS/device/n32g43x_flash.ld -Wl,--gc-sections -Wl,--print-memory-usage -specs=nano.specs -specs=nosys.specs"
  CACHE STRING "" FORCE
)

# Search behaviour for cross-compilation
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Optional convenience variables you can use in CMakeLists.txt
set(N32G435_CPU_FLAGS "${CPU_FLAGS}" CACHE STRING "CPU flags for N32G435")
