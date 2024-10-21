add_executable(basic_chakracore main.cpp)

target_link_libraries(basic_chakracore PRIVATE
    ChakraCoreStatic
)