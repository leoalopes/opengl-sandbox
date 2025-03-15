cmake_minimum_required(VERSION 3.24)

file(GLOB SRC_FILES "src/*.cpp" "src/ui/*.cpp")
add_executable(sandbox ${SRC_FILES})

find_package(glfw3 3.4 REQUIRED)
find_package(ImGui REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(sandbox glfw OpenGL::GL ImGui::imgui glad core)
