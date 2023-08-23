cmake_minimum_required(VERSION 3.5)

project(render LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (USING_OBSHPERE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRENDER_USING_OBSHPERE=1")
endif()
if (USING_CHECK_INTERSECT_MESH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRENDER_USING_CHECK_INTERSECT_MESH=1")
endif()


file(GLOB files_cpp "*.cpp")
file(GLOB files_h "*.h")

add_library(${PROJECT_NAME} STATIC ${files_cpp} ${files_h})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PRIVATE "../tinygltf")
