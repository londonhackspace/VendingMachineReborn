
if(DEFINED CONNECTED_LAUNCHPAD)
add_subdirectory(ConnectedLaunchpad)
elseif(DEFINED STNUCLEO144)
add_subdirectory(STNucleo144)
else()
add_subdirectory(pc)
endif()
