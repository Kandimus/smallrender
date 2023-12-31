cmake_minimum_required(VERSION 3.5)

add_subdirectory(render)

project(smallrender LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RENDER_USING_OBSHPERE "Using ObSphere for fast check intersection to triangles" ON)
option(RENDER_CHECK_INTERSECT_MESH "Using loop for mesh and after loop for meshes triangles" ON)
option(RENDER_CALCULATED_NORMALS "Using calcualting normal by vertices for triangles" ON)

if (RENDER_USING_OBSHPERE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRENDER_USING_OBSHPERE=1")
endif()
if (RENDER_CHECK_INTERSECT_MESH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRENDER_CHECK_INTERSECT_MESH=1")
endif()
if (RENDER_CALCULATED_NORMALS)
    add_compile_definitions(RENDER_CALCULATED_NORMALS)
endif()

file(GLOB files_simpleargs_cpp "simpleargs/simpleargs/src/*.cpp")
file(GLOB files_simpleargs_h "simpleargs/simpleargs/include/*.h")

file(GLOB files_tinygltf_cpp "tinygltf/tiny_gltf.cc")
file(GLOB files_tinygltf_h "tinygltf/tiny_gltf.h")

file(GLOB files_stb_cpp "")
file(GLOB files_stb_h "stb/*.h")

file(GLOB files_cpp "./*.cpp")


add_executable(${PROJECT_NAME} ${files_cpp}
    ${files_simpleargs_cpp} ${files_simpleargs_h}
    ${files_tinygltf_cpp} ${files_tinygltf_h}
    ${files_stb_cpp} ${files_stb_h}
)

target_include_directories(${PROJECT_NAME} PRIVATE "simpleargs/simpleargs/include")
target_include_directories(${PROJECT_NAME} PRIVATE "stb")
target_include_directories(${PROJECT_NAME} PRIVATE "tinygltf")

target_link_libraries(${PROJECT_NAME} PRIVATE render)
target_include_directories(${PROJECT_NAME} PRIVATE "render")



include(GNUInstallDirs)
install(TARGETS smallrender
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
