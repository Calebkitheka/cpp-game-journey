 # cpp-game-journey/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(CPPGameJourney LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 🔍 Find SFML (optional for CI runners without GPUs)
find_package(SFML 2.5 COMPONENTS graphics window system audio QUIET)

# 🔹 Helper to link SFML cleanly
function(add_sdl_game name src)
    add_executable(${name} ${src})
    if(SFML_FOUND)
        target_link_libraries(${name} PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)
    else()
        message(WARNING "SFML not found. Skipping ${name}")
    endif()
endfunction()

# 🔹 Console Exercises (Always build)
add_executable(stat_calc src/stat_calculator.cpp)
add_executable(damage_res src/damage_resolver.cpp)
add_executable(entity_mgr src/entity_manager_capstone.cpp)

# 🔹 Graphical Exercises (Build if SFML available)
if(SFML_FOUND)
    add_sdl_game(window_fw src/window_framework.cpp)
    add_sdl_game(movement src/realtime_movement.cpp)
    add_sdl_game(collision src/aabb_collision.cpp)
    add_sdl_game(pong src/pong_capstone.cpp)
endif()
