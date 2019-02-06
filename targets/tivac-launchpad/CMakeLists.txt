# build for core Energia libraries

file(GLOB TIVAC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/cores/tivac/*.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/cores/tivac/avr/*.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/cores/tivac/*.cpp)

add_library(tivac_lib ${TIVAC_SOURCES})
target_link_libraries(tivac_lib ${TIVA_SYSTEMLIB})

file(GLOB TIVAC_ETHERNET_SOURCES    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/Ethernet/*.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/Ethernet/*.c
                                    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/Ethernet/utility/*.c)

add_library(tivac_ethernet ${TIVAC_ETHERNET_SOURCES})
target_include_directories(tivac_ethernet PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/Ethernet)

target_link_libraries(tivac_ethernet tivac_lib)

file(GLOB TIVAC_EEPROM_SOURCES    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/EEPROM/*.cpp)

add_library(tivac_eeprom ${TIVAC_EEPROM_SOURCES})
target_include_directories(tivac_eeprom PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/EEPROM)

target_link_libraries(tivac_eeprom tivac_lib)

file(GLOB TIVAC_WIRE_SOURCES    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/Wire/*.cpp)

add_library(tivac_wire ${TIVAC_WIRE_SOURCES})
target_include_directories(tivac_wire PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/Wire)

target_link_libraries(tivac_wire tivac_lib)

file(GLOB TIVAC_SPI_SOURCES    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/SPI/*.cpp)

add_library(tivac_spi ${TIVAC_SPI_SOURCES})
target_include_directories(tivac_spi PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/libraries/SPI)

target_link_libraries(tivac_spi tivac_lib)