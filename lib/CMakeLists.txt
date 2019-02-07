
file(GLOB KEYPAD_SOURCES    ${CMAKE_CURRENT_SOURCE_DIR}/Keypad/src/*.cpp)

add_library(keypad ${KEYPAD_SOURCES})
target_include_directories(keypad PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/Keypad/src/)

file(GLOB PN532_SOURCES    ${CMAKE_CURRENT_SOURCE_DIR}/PN532/PN532/*.cpp)

add_library(PN532 ${PN532_SOURCES})
target_include_directories(PN532 PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/PN532/PN532/)

file(GLOB PN532_HSU_SOURCES    ${CMAKE_CURRENT_SOURCE_DIR}/PN532/PN532_HSU/*.cpp)

add_library(PN532_HSU ${PN532_HSU_SOURCES})
target_include_directories(PN532_HSU PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/PN532/PN532_HSU/)
target_link_libraries(PN532_HSU PN532)

file(GLOB ADAFRUIT_CHARACTER_OLED_SOURCES 
                       ${CMAKE_CURRENT_SOURCE_DIR}/Adafruit_CharacterOLED/*.cpp)

add_library(adafruit_character_oled ${ADAFRUIT_CHARACTER_OLED_SOURCES})
target_include_directories(adafruit_character_oled PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/Adafruit_CharacterOLED)

file(GLOB HTTPCLIENT_SOURCES 
                       ${CMAKE_CURRENT_SOURCE_DIR}/HttpClient/*.cpp)

add_library(httpclient ${HTTPCLIENT_SOURCES})
target_include_directories(httpclient PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/HttpClient)

add_library(arduinojson INTERFACE)
target_include_directories(arduinojson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ArduinoJson/src)