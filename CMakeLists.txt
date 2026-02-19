cmake_minimum_required(VERSION 3.10)
project(lab3)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(lab3 
  src/main.cpp 
  external/glad/src/glad.c
  external/imgui/imgui.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/imgui_impl_glfw.cpp
  external/imgui/imgui_impl_opengl3.cpp
)

target_include_directories(lab3 PRIVATE 
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/external/glad/include
  ${CMAKE_SOURCE_DIR}/external/imgui
)

add_compile_definitions(GL_SILENCE_DEPRECATION)

include_directories(/opt/homebrew/opt/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/external/glad/include)

link_directories(/opt/homebrew/opt/glfw/lib)
target_link_libraries(lab3 PRIVATE glfw assimp::assimp)

