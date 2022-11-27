cmake_minimum_required(VERSION 3.10)

project(Snake)

add_executable(Snake 
    src/snake.cpp 
    src/snake.hpp)

add_executable(Test 
    src/snakeTest.cpp 
    src/snake.hpp 
    src/catch_amalgamated.cpp 
    src/catch_amalgamated.hpp 
    src/test.cpp)
