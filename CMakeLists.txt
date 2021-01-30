cmake_minimum_required(VERSION 3.19)

project(Engine VERSION 0.0.0)

add_subdirectory(src)

target_include_directories(Engine PUBLIC /home/user/Development/vulkan-sdk/1.2.162.1/x86_64/include/)
target_link_libraries(Engine m)
target_link_libraries(Engine vulkan)
