add_executable(basic_wrapped_dukglue main.cpp)

target_link_libraries(basic_wrapped_dukglue PRIVATE
    dukglue
    duktape
)