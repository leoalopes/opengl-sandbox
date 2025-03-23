cmake_minimum_required(VERSION 3.24)

file(GLOB SRC_FILES "src/*.cpp" "src/ui/*.cpp")
add_executable(sandbox ${SRC_FILES})

target_link_libraries(sandbox core)
