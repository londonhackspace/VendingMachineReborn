cmake_minimum_required (VERSION 3.1)
project (VendingMachineReborn)

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
endif()

add_subdirectory(src)
