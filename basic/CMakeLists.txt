add_subdirectory(ChakraCore)
add_subdirectory(duktape)
add_subdirectory(Escargot)
add_subdirectory(jerryscript)
add_subdirectory(JUCE7)
add_subdirectory(JUCE8)
add_subdirectory(quickjs)
add_subdirectory(quickjs-ng)

add_subdirectory(JavascriptCore)
add_subdirectory(v8)

if (WITH_STATIC_HERMES)
    add_subdirectory(static-hermes)
else()
    add_subdirectory(hermes)
endif ()

add_subdirectory(wrapped)