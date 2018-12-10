cmake_minimum_required(VERSION 2.6)
project(matrixscreen)

add_executable(matrixscreen main.cpp)

link_directories(/usr/local/lib)
target_link_libraries(matrixscreen sfml-graphics sfml-window sfml-system)
