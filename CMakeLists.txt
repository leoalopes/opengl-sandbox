cmake_minimum_required(VERSION 3.24)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "c++")

project(OpenGLSandbox VERSION 1.0 LANGUAGES C CXX)
add_compile_definitions(PROJECT_DIRECTORY="${CMAKE_SOURCE_DIR}/")

add_subdirectory(dependencies)
add_subdirectory(core)
add_subdirectory(app)

