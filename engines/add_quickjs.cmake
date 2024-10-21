function(add_quickjs quickjs_dir)
    set(quickjs_src
            ${quickjs_dir}/quickjs.c
            ${quickjs_dir}/libbf.c
            ${quickjs_dir}/libunicode.c
            ${quickjs_dir}/libregexp.c
            ${quickjs_dir}/cutils.c
            ${quickjs_dir}/cutils.h
            ${quickjs_dir}/quickjs-libc.c
    )
    set(quickjs_def CONFIG_VERSION="${version}" _GNU_SOURCE CONFIG_BIGNUM)

    add_library(quickjs ${quickjs_src})
    target_include_directories(quickjs PUBLIC ${quickjs_dir})
    target_compile_definitions(quickjs PRIVATE ${quickjs_def} )

    if(UNIX)
        find_package(Threads)
        target_link_libraries(quickjs PRIVATE ${CMAKE_DL_LIBS} m Threads::Threads)
    elseif(WIN32)
        target_compile_definitions(quickjs PUBLIC JS_STRICT_NAN_BOXING)
        set_property(TARGET quickjs PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endfunction()
