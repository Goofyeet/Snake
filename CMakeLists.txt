cmake_minimum_required(VERSION 3.10)

project(Snake)

set(SFML_DIR C:/Libs/SFML-2.5.1/lib/cmake/SFML)

find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

add_executable(Snake 
    src/snake.cpp 
    src/snake.hpp)

target_include_directories(Snake PRIVATE "C:/Libs/SFML-2.5.1/include")

target_link_libraries(Snake sfml-graphics)    

