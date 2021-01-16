cmake_minimum_required(VERSION 3.19)

project(Engine VERSION 0.0.0)

add_subdirectory(src)

target_link_libraries(PRIVATE m)
