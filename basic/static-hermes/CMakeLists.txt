add_executable(basic_static-hermes main.cpp)

target_link_libraries(basic_static-hermes PRIVATE
    hermesvm_a
)