set(CMAKE_EXE_LINKER_FLAGS "-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant -nostartfiles -T ${LINKER_SCRIPT}" CACHE STRING "")

set(CMAKE_C_FLAGS "-g -Os -ffunction-sections -fdata-sections --param max-inline-insns-single=500 -Dprintf=iprintf -MMD -DF_CPU=120000000L -DARDUINO=100 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant -specs=nano.specs" CACHE STRING "")
set(CMAKE_CXX_FLAGS "-g -Os -ffunction-sections -fdata-sections -fno-threadsafe-statics --param max-inline-insns-single=500 -fno-rtti -fno-exceptions -Dprintf=iprintf -MMD -DF_CPU=120000000L -DARDUINO=100 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant -specs=nano.specs"  CACHE STRING "")

set(ENERGIA_CORE ${CMAKE_SOURCE_DIR}/targets/tivac-launchpad/tivac-core)
set(TIVA_SYSTEMLIB ${CMAKE_SOURCE_DIR}/targets/tivac-launchpad/tivac-core/system/driverlib/libdriverlib.a)
include_directories(${CMAKE_SOURCE_DIR}/targets/tivac-launchpad/tivac-core/system/inc)
include_directories(${CMAKE_SOURCE_DIR}/targets/tivac-launchpad/tivac-core/system)
include_directories(${CMAKE_SOURCE_DIR}/targets/tivac-launchpad/tivac-core/cores/tivac)

set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(SYSTEM_LIBRARY ConnectedLaunchpad::Core)

set(CONNECTED_LAUNCHPAD 1)

function(makeFlash TARGET)
    add_custom_target(flash 
        COMMAND ${CMAKE_SOURCE_DIR}/toolchain/DSLite/DebugServer/bin/DSLite flash -c ${DSLITE_CONFIG} -f $<TARGET_FILE:${TARGET}>)
endfunction(makeFlash)