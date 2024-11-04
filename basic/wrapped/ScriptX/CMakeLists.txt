add_executable(basic_wrapped_scriptx main.cpp)

target_link_libraries(basic_wrapped_scriptx PRIVATE
    ScriptX
)

if ($CACHE{SCRIPTX_BACKEND} STREQUAL "JavaScriptCore")
    if (APPLE)
        target_link_libraries(ScriptX INTERFACE "-framework JavaScriptCore")
    else ()
        add_subdirectory(${PROJECT_SOURCE_DIR}/engines/JavascriptCore)
        target_link_libraries(ScriptX PUBLIC JavaScriptCore)
        target_include_directories(ScriptX PUBLIC ${CMAKE_BINARY_DIR}/JavaScriptCore/Headers)
    endif ()
    target_compile_definitions(ScriptX PUBLIC
        SCRIPTX_BACKEND_TRAIT_PREFIX=../backend/JavaScriptCore/trait/Trait
    )
elseif ($CACHE{SCRIPTX_BACKEND} STREQUAL "QuickJs")
    if(TARGET qjs)
        target_compile_definitions(ScriptX PUBLIC
            QUICKJS_NG=1
        )
        target_link_libraries(ScriptX PUBLIC qjs)
    endif ()

    target_compile_definitions(ScriptX PUBLIC
        SCRIPTX_BACKEND_TRAIT_PREFIX=../backend/QuickJs/trait/Trait
    )
elseif (WITH_V8 AND $CACHE{SCRIPTX_BACKEND} STREQUAL "V8")
    target_link_libraries(ScriptX PUBLIC
        v8_base_without_compiler
        v8_compiler
        v8_snapshot
        v8_libplatform
    )
    target_compile_definitions(ScriptX PUBLIC
        SCRIPTX_BACKEND_TRAIT_PREFIX=../backend/V8/trait/Trait
    )
endif()