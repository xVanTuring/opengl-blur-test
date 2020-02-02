cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "/home/xvan/Project/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(OPENGL_Learn)
add_executable(2d 2d.cc)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(2d PRIVATE glm)
target_link_libraries(2d PRIVATE glad::glad)
target_link_libraries(2d PRIVATE glfw)
