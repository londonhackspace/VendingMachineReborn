cmake_minimum_required (VERSION 3.1)
project (VendingMachineReborn)

include(cmake/AddVendorLibrary.cmake)

if(DEFINED CONNECTED_LAUNCHPAD)
    add_subdirectory(targets/tivac-launchpad/)
elseif(DEFINED STM32)
    add_subdirectory(targets/stm32/)
endif()

add_subdirectory(lib)

add_executable(VendingMachineReborn src/main.cpp)
target_link_libraries(VendingMachineReborn ${SYSTEM_LIBRARY} keypad PN532_HSU adafruit_character_oled httpclient arduinojson)

makeFlash(VendingMachineReborn)