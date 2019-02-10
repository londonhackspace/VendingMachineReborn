
set(DUMMY_DISPLAY_SOURCES DummyDisplay.cpp)

add_library(DummyDisplay ${DUMMY_DISPLAY_SOURCES})

# add an alias to make the rest of the scripts easier to write
add_library(Display ALIAS DummyDisplay)
