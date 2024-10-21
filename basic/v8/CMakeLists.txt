if (WITH_V8)
    add_executable(basic_v8 main.cpp)

    target_link_libraries(basic_v8 PRIVATE
        v8_base_without_compiler
        v8_compiler
        v8_snapshot
        v8_libplatform
    )
endif()
