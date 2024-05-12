cmake_minimum_required(VERSION 3.0)
project(Minesweeper)

set(CMAKE_BUILD_TYPE Debug)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(SOURCES
    src/main.cpp
    src/game.cpp
    src/tile.cpp
    src/gui.cpp
)

add_executable(Minesweeper ${SOURCES})

target_link_libraries(Minesweeper 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio
)

target_compile_options(Minesweeper PRIVATE -g)
