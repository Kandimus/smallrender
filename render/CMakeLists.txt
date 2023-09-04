cmake_minimum_required(VERSION 3.5)

project(render LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (RENDER_USING_OBSHPERE)
    add_compile_definitions(RENDER_USING_OBSHPERE)
endif()
if (RENDER_CHECK_INTERSECT_MESH)
    add_compile_definitions(RENDER_CHECK_INTERSECT_MESH)
endif()
if (RENDER_CALCULATED_NORMALS)
    add_compile_definitions(RENDER_CALCULATED_NORMALS)
endif()


file(GLOB files_cpp "*.cpp")
file(GLOB files_h "*.h")

add_library(${PROJECT_NAME} STATIC ${files_cpp} ${files_h})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} PRIVATE "../tinygltf")
