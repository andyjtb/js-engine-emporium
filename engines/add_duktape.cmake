function(add_duktape duktape_dir)
    set(duktape_built ${duktape_dir}/built_${CMAKE_BUILD_TYPE})
    if (NOT EXISTS ${duktape_built})
        find_package(Python COMPONENTS Interpreter REQUIRED)
        execute_process(
            COMMAND "${Python_EXECUTABLE}" "${duktape_dir}/util/dist.py" "--output-directory=built_${CMAKE_BUILD_TYPE}"
            WORKING_DIRECTORY "${duktape_dir}"
            RESULT_VARIABLE PYTHON_RESULT
            OUTPUT_VARIABLE PYTHON_OUTPUT
            ERROR_VARIABLE PYTHON_ERROR
        )

        if(NOT ${PYTHON_RESULT} EQUAL 0)
            message(FATAL_ERROR "Failed to run dist.py:\n${PYTHON_OUTPUT}\n${PYTHON_ERROR}")
        endif()
    endif()
    set(duktape_src
        ${duktape_built}/src/duk_config.h
        ${duktape_built}/src/duktape.c
        ${duktape_built}/src/duktape.h
    )

    add_library(duktape ${duktape_src})
    target_include_directories(duktape PUBLIC ${duktape_built}/src)
endfunction()
