cmake_minimum_required (VERSION 3.1)
project (VendingMachineRebord)

if(DEFINED ENERGIA_CORE)
    add_subdirectory(targets/tivac-launchpad/)
endif()