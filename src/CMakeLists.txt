
include_directories(Core/include)

add_subdirectory(Core)
add_subdirectory(HAL)
add_subdirectory(Modules)

add_executable(VendingMachineReborn main.cpp)
target_link_libraries(VendingMachineReborn Core ${SYSTEM_LIBRARY})
makeFlash(VendingMachineReborn)