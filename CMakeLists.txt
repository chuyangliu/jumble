cmake_minimum_required(VERSION 3.10)

project(jumble)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

add_subdirectory(${PROJECT_SOURCE_DIR}/c)
add_subdirectory(${PROJECT_SOURCE_DIR}/cpp)
