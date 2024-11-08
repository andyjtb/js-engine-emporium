if (WITH_V8)
    add_subdirectory(v8-cmake)
endif()

add_subdirectory(JUCE)

set(STATIC_LIBRARY 1)
add_subdirectory(ChakraCore)

set(ESCARGOT_OUTPUT "static_lib")
add_subdirectory(escargot)

add_subdirectory(jerryscript)

if (WITH_QUICKJSNG)
    set(BUILD_QJS_LIBC ON)
    add_subdirectory(quickjs-ng)
else()
    include(add_quickjs.cmake)
    add_quickjs(quickjs)
endif()

if (WITH_STATIC_HERMES)
    add_subdirectory(static-hermes)
else()
    add_subdirectory(hermes)
endif ()

include(add_duktape.cmake)
add_duktape(${CMAKE_CURRENT_LIST_DIR}/duktape)
