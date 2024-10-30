add_definitions("-DV8_ENABLE_TURBOFAN")

if (WITH_V8)
    add_subdirectory(v8-cmake)
endif()

add_subdirectory(JUCE)

set(STATIC_LIBRARY 1)
add_subdirectory(ChakraCore)

set(ESCARGOT_OUTPUT "static_lib")
add_subdirectory(escargot)

add_subdirectory(jerryscript)

add_subdirectory(quickjs-ng)

include(add_quickjs.cmake)
add_quickjs(quickjs)

if (WITH_STATIC_HERMES)
    add_subdirectory(static-hermes)
else()
    add_subdirectory(hermes)
endif ()

include(add_duktape.cmake)
add_duktape(${CMAKE_CURRENT_LIST_DIR}/duktape)
