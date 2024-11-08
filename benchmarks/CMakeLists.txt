add_subdirectory(bytecode)

add_executable(benchmarks main.cpp)

target_link_libraries(benchmarks PRIVATE
    Catch2::Catch2WithMain

    ChakraCoreStatic

    escargot

    juce::juce_core
    juce7_js

    jerry-core
    jerry-ext
    jerry-port

    ScriptX
)

if (APPLE)
    target_link_libraries(benchmarks PUBLIC "-framework JavaScriptCore")
endif()

if (WITH_STATIC_HERMES)
    target_link_libraries(benchmarks PRIVATE
        hermesvm_a
    )
else()
    target_link_libraries(benchmarks PRIVATE
        libhermes
    )
endif()

target_include_directories(benchmarks PUBLIC
    ${PROJECT_SOURCE_DIR}/basic
)

set(WITH_CHOC_TYPE "V8")

if (WITH_CHOC_TYPE STREQUAL "Duktape")
    target_compile_definitions(benchmarks PUBLIC
        WITH_CHOC_DUKTAPE=1
    )
    target_link_libraries(benchmarks PUBLIC
        choc_js
    )
else()
    target_link_libraries(benchmarks PUBLIC
        duktape
    )
endif ()

if (WITH_CHOC_TYPE STREQUAL "QuickJS")
    target_compile_definitions(benchmarks PUBLIC
        WITH_CHOC_QUICKJS=1
    )
    target_link_libraries(benchmarks PUBLIC
        choc_js
    )
else()
    target_link_libraries(benchmarks PUBLIC
        quickjs
    )
endif ()

if (WITH_V8)
    target_compile_definitions(benchmarks PUBLIC
        WITH_V8=1
    )
    target_link_libraries(benchmarks PRIVATE
        v8_base_without_compiler
        v8_compiler
        v8_snapshot
        v8_libplatform
    )
    if (WITH_CHOC_TYPE STREQUAL "V8")
        target_compile_definitions(benchmarks PUBLIC
            WITH_CHOC_V8=1
        )
        target_link_libraries(benchmarks PUBLIC
            choc_js
        )
    endif()
endif ()
