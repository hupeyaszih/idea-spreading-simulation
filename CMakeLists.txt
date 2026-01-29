cmake_minimum_required(VERSION 3.16)

project(ideological-compass-model LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ideological-compass-model
    src/main.c
    src/simulation.c
)

target_link_libraries(ideological-compass-model m)

target_include_directories(ideological-compass-model
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
