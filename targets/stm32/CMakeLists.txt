# Core STM32Duino libraries

enable_language(ASM)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET Core
    FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/avr/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/stm32/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/stm32/*.S
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/stm32/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/stm32/HAL/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/stm32/LL/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/stm32/usb/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/stm32/usb/cdc/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/stm32/usb/hid/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/cores/arduino/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/system/STM32H7xx/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/variants/NUCLEO_H743ZI/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/variants/NUCLEO_H743ZI/*.cpp
)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET CapacitiveSensor
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/CapacitiveSensor-0.5.1/*.cpp
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/CapacitiveSensor-0.5.1
    DEPENDENCIES
        stm32::Core
)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET EEPROM
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/EEPROM/src
    DEPENDENCIES
        stm32::Core
)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET IWatchdog
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/IWatchdog/src/*.cpp
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/IWatchdog/src
    DEPENDENCIES
        stm32::Core
)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET Keyboard
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/Keyboard/src/*.cpp
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/Keyboard/src
    DEPENDENCIES
        stm32::Core
)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET Mouse
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/Mouse/src/*.cpp
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/Mouse/src
    DEPENDENCIES
        stm32::Core
)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET Servo
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/Servo/src/stm32/*.cpp
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/Servo/src
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/Servo/src/stm32
    DEPENDENCIES
        stm32::Core
)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET SPI
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/SPI/src/*.cpp
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/SPI/src
    DEPENDENCIES
        stm32::Core
)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET Wire
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/Wire/src/*.cpp
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/Arduino_Core_STM32/libraries/Wire/src
    DEPENDENCIES
        stm32::Core
)

AddVendorLibrary(
    NAMESPACE stm32
    TARGET LwIP
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP/src/api/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP/src/core/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP/src/core/ipv4/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP/src/core/ipv6/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP/src/netif/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP/src/netif/ppp/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP/src/netif/ppp/polarssl/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP/src/system/OS/*.c
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP
        ${CMAKE_CURRENT_SOURCE_DIR}/LwIP/src
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Ethernet/src
    DEPENDENCIES
        stm32::Core
)

# TODO: This doesn't currently work!
# Fix Me!
AddVendorLibrary(
    NAMESPACE stm32
    TARGET Ethernet
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Ethernet/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Ethernet/src/utility/*.c
    INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/STM32Ethernet/src
    DEPENDENCIES
        stm32::Core
        stm32::LwIP
)
