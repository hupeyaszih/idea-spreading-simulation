
cmake_minimum_required(VERSION 3.16)

project(idea_simulation LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Core library (C) ---
add_library(idea_simulation_core
    core/src/simulation.c
)

target_include_directories(idea_simulation_core
    PUBLIC
        core/include
)

target_link_libraries(idea_simulation_core
    PRIVATE
        m
)

# --- SDL2 ---
find_package(SDL2 REQUIRED)

# --- Visualization (C++) ---
add_executable(idea_simulation_vis
    visual/src/main.cpp
    visual/src/renderer.cpp
)

target_include_directories(idea_simulation_vis
    PRIVATE
        visual/include
)
target_link_libraries(idea_simulation_vis
    PRIVATE
        idea_simulation_core
        SDL2::SDL2
)
