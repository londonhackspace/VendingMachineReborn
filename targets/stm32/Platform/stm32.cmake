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
    #add_custom_target(flash 
    #    COMMAND ${CMAKE_SOURCE_DIR}/toolchain/DSLite/DebugServer/bin/DSLite flash -c ${DSLITE_CONFIG} -f $<TARGET_FILE:${TARGET}>)
endfunction(makeFlash)