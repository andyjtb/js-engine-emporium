add_executable(basic_quickjs-ng main.cpp)

target_link_libraries(basic_quickjs-ng PRIVATE
    qjs
)