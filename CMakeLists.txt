cmake_minimum_required(VERSION 3.0.0)
project(simple3d VERSION 0.0.1)

add_executable(simple3d_demo 
    src/main.c 
    src/Matrix.c
    src/Vector.c
    src/Model.c
    src/Camera.c)

target_include_directories(simple3d_demo PRIVATE 
    Matrix.h
    Vector.h
    Model.h
    Camera.h)

target_link_libraries(simple3d_demo SDL2main SDL2 m)
