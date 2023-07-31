cmake_minimum_required(VERSION 3.5)

project(render LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB files_cpp "*.cpp")
file(GLOB files_h "*.h")

add_library(${PROJECT_NAME} STATIC ${files_cpp} ${files_h})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
