
include_directories(Core/include)

if(CMAKE_CROSSCOMPILING)
    # Default module config for real hardware
    set(DEFAULT_DISPLAY_TYPE "DUMMY")
    set(DEFAULT_CARD_READER_TYPE "DUMMY")
    set(DEFAULT_VEND_INTERFACE_TYPE "DUMMY")
else()
    # Default module config for desktop builds
    set(DEFAULT_DISPLAY_TYPE "DUMMY")
    set(DEFAULT_CARD_READER_TYPE "DUMMY")
    set(DEFAULT_VEND_INTERFACE_TYPE "DUMMY")
endif()

# include optional config file to override the defaults
if(DEFINED MACHINE_CONFIG_FILE)
    include(${MACHINE_CONFIG_FILE})
endif()

# Module choice options
set(DISPLAY_TYPE ${DEFAULT_DISPLAY_TYPE} CACHE STRING "Type of display in use")
set(CARD_READER_TYPE ${DEFAULT_CARD_READER_TYPE} CACHE STRING "Type of card reader in use")
set(VEND_INTERFACE_TYPE ${DEFAULT_VEND_INTERFACE_TYPE} CACHE STRING "Type of motor interface in use")

add_subdirectory(Core)
add_subdirectory(HAL)
add_subdirectory(Modules)

configure_file(VendingMachineConfig.h.template VendingMachineConfig.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(VendingMachineReborn main.cpp)
target_link_libraries(VendingMachineReborn Core ${SYSTEM_LIBRARY})
makeFlash(VendingMachineReborn)
