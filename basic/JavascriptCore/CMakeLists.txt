add_executable(basic_javascriptcore main.cpp)

if (APPLE)
    target_link_libraries(basic_javascriptcore PUBLIC "-framework JavaScriptCore")
else ()
    target_link_libraries(basic_javascriptcore PUBLIC JavaScriptCore)
    target_include_directories(basic_javascriptcore PUBLIC ${CMAKE_BINARY_DIR}/JavaScriptCore/Headers)
endif ()
