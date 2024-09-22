cmake_minimum_required(VERSION 3.29)
project(Kuznechik C)

set(CMAKE_C_STANDARD 17)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_Release ON)

add_compile_options(
        $<$<CONFIG:Release>:-Ofast>
        $<$<CONFIG:Release>:-march=native>
        $<$<CONFIG:Release>:-funroll-loops>
        $<$<CONFIG:Release>:-falign-functions>
        $<$<CONFIG:Release>:-ffast-math>
        $<$<CONFIG:Release>:-fno-math-errno>
        $<$<CONFIG:Release>:-fno-trapping-math>
)

add_library(encryption STATIC funcs.c)
target_include_directories(encryption PUBLIC include)

add_executable(kuznechik main.c)
target_link_libraries(kuznechik encryption)

add_subdirectory(testing)
