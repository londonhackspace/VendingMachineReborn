set(CMAKE_EXECUTABLE_SUFFIX .elf)

include_directories(${PROJECT_SOURCE_DIR}/targets/stm32/Arduino_Core_STM32/cores/arduino)
include_directories(${PROJECT_SOURCE_DIR}/targets/stm32/Arduino_Core_STM32/cores/arduino/stm32)
include_directories(${PROJECT_SOURCE_DIR}/targets/stm32/Arduino_Core_STM32/cores/arduino/stm32/LL)
include_directories(${PROJECT_SOURCE_DIR}/targets/stm32/Arduino_Core_STM32/cores/arduino/stm32/HAL)
include_directories(${PROJECT_SOURCE_DIR}/targets/stm32/CMSIS_5/CMSIS/Core/Include)

set(STM32 1)
set(SYSTEM_LIBRARY stm32::Core)

set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS "${CMAKE_ASM_SOURCE_FILE_EXTENSIONS} .S")

function(makeFlash TARGET)
    add_custom_target(${TARGET}.bin
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}> $<TARGET_FILE:${TARGET}>.bin
        DEPENDS ${TARGET})
    file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/upload-commands.cfg 
            CONTENT "init
reset init
halt
flash write_image erase $<TARGET_FILE:${TARGET}>.bin 0x08000000
reset
shutdown
")
    add_custom_target(flash 
        COMMAND ${CMAKE_SOURCE_DIR}/toolchain/stm32-tools/bin/openocd -f ${CMAKE_SOURCE_DIR}/toolchain/stm32-tools/scripts/board/${STLINK_CONFIG} -f ${CMAKE_CURRENT_BINARY_DIR}/upload-commands.cfg
        DEPENDS ${TARGET}.bin
        )
endfunction(makeFlash)