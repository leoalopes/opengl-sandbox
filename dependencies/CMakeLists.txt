cmake_minimum_required(VERSION 3.24)

add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)

add_library(stb STATIC stb/stb.c)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb/include)

add_library(tiny_gltf STATIC tiny_gltf/tiny_gltf.cpp)
target_include_directories(tiny_gltf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tiny_gltf/include)
target_link_libraries(tiny_gltf stb)
