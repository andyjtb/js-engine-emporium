juce_add_console_app(basic_juce8)

target_sources(basic_juce8 PRIVATE
    Main.cpp
)
target_link_libraries(basic_juce8 PRIVATE
    juce::juce_core
)