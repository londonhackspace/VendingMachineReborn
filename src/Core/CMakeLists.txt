
set(CORE_SOURCES
    Core.cpp
    destructors.cpp
    ConfigManager.cpp
    FSM.cpp
)

add_library(Core ${CORE_SOURCES})

if(TESTING)
    add_subdirectory(tests)
endif()