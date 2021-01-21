set(CMAKE_SYSTEM_NAME stm32)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

# Ideally this would be in the binary directory, however
# when it runs tests it makes a mess of that
set(TOOLCHAIN_DIR ${PROJECT_SOURCE_DIR}/toolchain/Connected_Launchpad)

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/arm-none-eabi-g++)
set(CMAKE_OBJCOPY ${TOOLCHAIN_DIR}/bin/arm-none-eabi-objcopy)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/Arduino_Core_STM32/system/Drivers/STM32H7xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_LIST_DIR}/Arduino_Core_STM32/system/Drivers/STM32H7xx_HAL_Driver/Src
    ${CMAKE_CURRENT_LIST_DIR}/Arduino_Core_STM32/system/Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ${CMAKE_CURRENT_LIST_DIR}/Arduino_Core_STM32/variants/NUCLEO_H743ZI
    ${CMAKE_CURRENT_LIST_DIR}/Arduino_Core_STM32/system/STM32H7xx/
    ${CMAKE_CURRENT_LIST_DIR}/Arduino_Core_STM32/system/Drivers/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/Arduino_Core_STM32/variants/NUCLEO_H743ZI/ldscript.ld)

# TODO: Tune these - they're Launchpad ones butchered
set(CMAKE_C_FLAGS "-g -Os -ffunction-sections -fdata-sections -ffreestanding --param max-inline-insns-single=500 -MMD -mcpu=cortex-m7 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -specs=nano.specs " CACHE STRING "")

set(CMAKE_CXX_FLAGS "-g -Os -ffunction-sections -fdata-sections -ffreestanding --param max-inline-insns-single=500 -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics  -MMD  -mcpu=cortex-m7 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb  -specs=nano.specs "  CACHE STRING "")

add_definitions(
                    -DBOARD_NAME=NUCLEO_H743ZI
                    -DARDUINO_NUCLEO_H743ZI
                    -DSTM32H7xx 
                    -DSTM32H743xx
                )

set(STNUCLEO144 1)
set(NUCLEO_H743ZI 1)

set(CMAKE_EXE_LINKER_FLAGS "-mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant -ffreestanding -flto -T ${LINKER_SCRIPT}  -Wl,--gc-sections -Wl,--entry=Reset_Handler " CACHE STRING "")

set(STLINK_CONFIG st_nucleo_h743zi.cfg)
