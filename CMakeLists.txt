cmake_minimum_required(VERSION 3.5)

project(smallrender LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB files_cpp "./*.cpp")

add_executable(smallrender ${files_cpp})

include(GNUInstallDirs)
install(TARGETS smallrender
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
