add_executable(basic_duktape main.cpp)

target_link_libraries(basic_duktape PRIVATE
    duktape
)