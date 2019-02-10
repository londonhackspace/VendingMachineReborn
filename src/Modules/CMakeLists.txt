

# Choose a display type
if(DISPLAY_TYPE STREQUAL "DUMMY")
    set(DISPLAY_TYPE_DUMMY 1 PARENT_SCOPE)
    add_subdirectory(display/Dummy)
else()
    message(FATAL_ERROR "Unknown display type ${DISPLAY_TYPE}")
endif()

if(CARD_READER_TYPE STREQUAL "DUMMY")
    set(CARD_READER_TYPE_DUMMY 1 PARENT_SCOPE)
    # TODO: Implement dummy card reader
else()
    message(FATAL_ERROR "Unknown card reader type ${CARD_READER_TYPE}")
endif()

if(VEND_INTERFACE_TYPE STREQUAL "DUMMY")
    set(VEND_INTERFACE_TYPE_DUMMY 1 PARENT_SCOPE)
    # TODO: Implement dummy vend interface
else()
    message(FATAL_ERROR "Unknown vend interface type ${VEND_INTERFACE_TYPE}")
endif()