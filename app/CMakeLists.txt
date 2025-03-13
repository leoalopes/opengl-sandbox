cmake_minimum_required(VERSION 3.24)

file(GLOB SRC_FILES "src/*.cpp" "src/ui/*.cpp")
add_executable(sandbox ${SRC_FILES})

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
set(TARGET_DIR ${CMAKE_BINARY_DIR}/app/assets)
add_custom_target(copy_assets ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_DIR} ${TARGET_DIR}
  COMMENT "Copying assets to build directory"
)

add_dependencies(sandbox copy_assets)

find_package(glfw3 3.4 REQUIRED)
find_package(ImGui REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(sandbox glfw OpenGL::GL ImGui::imgui glad core)
