cmake_minimum_required(VERSION 3.16)

project(JsEngines VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(engines)
add_subdirectory(wrappers)

add_subdirectory(Catch2)
add_subdirectory(benchmarks)

add_subdirectory(basic)
