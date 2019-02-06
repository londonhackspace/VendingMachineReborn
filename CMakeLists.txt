cmake_minimum_required (VERSION 3.1)
project (VendingMachineRebord)

if(DEFINED ENERGIA_CORE)
    add_subdirectory(targets/tivac-launchpad/)
endif()

add_subdirectory(lib)

add_executable(VendingMachineReborn src/main.cpp)
target_link_libraries(VendingMachineReborn tivac_lib tivac_ethernet mysql_connector_arduino keypad PN532_HSU tivac_spi adafruit_character_oled httpclient arduinojson)

makeFlash(VendingMachineReborn)