cmake_minimum_required(VERSION 3.0)
project(Minesweeper)

# Adicione a opção de compilação para o modo de depuração
set(CMAKE_BUILD_TYPE Debug)

# Encontre as bibliotecas SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Defina as fontes do seu projeto
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/tile.cpp
    src/gui.cpp
)

# Adicione o executável
add_executable(Minesweeper ${SOURCES})

# Conecte as bibliotecas SFML ao executável
target_link_libraries(Minesweeper 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio
)

# Adicione a flag de compilação -g para garantir informações de depuração
target_compile_options(Minesweeper PRIVATE -g)
