juce_add_console_app(basic_juce7)

add_library(juce7_js INTERFACE)
target_sources(juce7_js INTERFACE
    ${PROJECT_SOURCE_DIR}/engines/JUCE7/juce_Javascript.cpp
)

target_include_directories(juce7_js INTERFACE
    ${PROJECT_SOURCE_DIR}/engines/JUCE7/
)

target_sources(basic_juce7 PRIVATE
    Main.cpp
)

target_link_libraries(basic_juce7 PRIVATE
    juce::juce_core
    juce7_js
)