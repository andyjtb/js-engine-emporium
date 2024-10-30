add_executable(basic_jerryscript main.cpp)

target_link_libraries(basic_jerryscript PRIVATE
    jerry-core
    jerry-ext
    jerry-port
)