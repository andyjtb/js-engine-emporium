add_executable(benchmarks_bytecode main.cpp)

target_link_libraries(benchmarks_bytecode PRIVATE
    Catch2::Catch2WithMain
    ScriptX
    qjs
)

if ($CACHE{SCRIPTX_BACKEND} STREQUAL "QuickJs")
    target_link_libraries(ScriptX PRIVATE
        qjs
    )
    target_include_directories(benchmarks_bytecode PRIVATE
        ${PROJECT_SOURCE_DIR}/engines/quickjs-ng
    )
else ()
    if (TARGET hermesvm_a)
        set(hermes_lib hermesvm_a)
    else()
        set(hermes_lib libhermes)
    endif()

    target_link_libraries(benchmarks_bytecode PRIVATE
        ${hermes_lib}
    )
endif ()

target_include_directories(benchmarks_bytecode PUBLIC
    ${PROJECT_SOURCE_DIR}/basic
)
