cmake_minimum_required(VERSION 3.24)

file(GLOB_RECURSE CORE_FILES "src/*.cpp")
add_library(core STATIC ${CORE_FILES})

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(core glfw)
target_link_libraries(core stb)
target_link_libraries(core glad)
target_link_libraries(core tiny_gltf)
target_link_libraries(core OpenGL::GL)
target_link_libraries(core ImGui::imgui)

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/dependencies/glm)
