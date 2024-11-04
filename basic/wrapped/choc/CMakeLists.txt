add_executable(basic_wrapped_choc main.cpp)

target_link_libraries(basic_wrapped_choc PRIVATE
    choc_js
)

if (WITH_V8)
    target_link_libraries(basic_wrapped_choc PRIVATE
        v8_base_without_compiler
        v8_compiler
        v8_snapshot
        v8_libplatform
    )

    target_compile_definitions(basic_wrapped_choc PUBLIC
        WITH_V8=1
    )
endif()