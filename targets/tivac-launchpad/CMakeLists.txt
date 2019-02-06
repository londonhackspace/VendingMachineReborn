# build for core Energia libraries

file(GLOB TIVAC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/cores/tivac/*.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/cores/tivac/avr/*.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/tivac-core/cores/tivac/*.cpp)

add_library(tivac_lib ${TIVAC_SOURCES})
target_link_libraries(tivac_lib ${TIVA_SYSTEMLIB})
