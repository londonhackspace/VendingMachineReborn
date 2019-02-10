cmake_minimum_required (VERSION 3.1)
project (VendingMachineReborn)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

include(cmake/AddVendorLibrary.cmake)

if(DEFINED CONNECTED_LAUNCHPAD)
    add_subdirectory(targets/tivac-launchpad/)
elseif(DEFINED STM32)
    add_subdirectory(targets/stm32/)
else()
    # Desktop build
    function(makeFlash)

    endfunction()
endif()


if(CMAKE_CROSSCOMPILING)
    add_subdirectory(lib)
    set(TESTING OFF)
else()
    enable_testing()
    add_subdirectory(lib/googletest)
    set(TESTING ON)
endif()

add_subdirectory(src)
