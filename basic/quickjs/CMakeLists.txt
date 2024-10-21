add_executable(basic_quickjs main.cpp)

target_link_libraries(basic_quickjs PRIVATE
    quickjs
)