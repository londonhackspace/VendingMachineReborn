
set(TEST_SRCS
    FSMTests.cpp
    ConfigManagerTests.cpp
)

add_executable(CoreTests ${TEST_SRCS})
target_link_libraries(CoreTests Core gtest gmock gmock_main)
add_test(Core CoreTests)