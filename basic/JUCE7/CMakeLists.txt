juce_add_console_app(basic_juce7)

target_sources(basic_juce7 PRIVATE
    Main.cpp
    ${PROJECT_SOURCE_DIR}/engines/JUCE7/juce_Javascript.cpp
)

target_include_directories(basic_juce7 PRIVATE
    ${PROJECT_SOURCE_DIR}/engines/JUCE7/
)

target_link_libraries(basic_juce7 PRIVATE
    juce::juce_core
)