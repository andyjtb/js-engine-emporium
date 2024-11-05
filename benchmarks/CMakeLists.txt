add_executable(benchmarks main.cpp)

target_link_libraries(benchmarks PRIVATE
    Catch2::Catch2WithMain

    ChakraCoreStatic

    duktape

    escargot

    quickjs

    juce::juce_core
    juce7_js

    jerry-core
    jerry-ext
    jerry-port
)

if (APPLE)
    target_link_libraries(benchmarks PUBLIC "-framework JavaScriptCore")
endif()

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
