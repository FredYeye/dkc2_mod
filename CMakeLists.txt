cmake_minimum_required(VERSION 2.8.3)
set(project_name oof)
project(${project_name})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(source_files
	src/main.cpp
	src/dkc2.cpp
	src/file.cpp
	src/imgui/imgui.cpp
	src/imgui/imgui_draw.cpp
	src/imgui/imgui_demo.cpp
	src/imgui/imgui_widgets.cpp
	src/imgui/examples/imgui_impl_glfw.cpp
	src/imgui/examples/imgui_impl_opengl3.cpp
	src/gl_core/gl_core_3_3.c
	)

set(header_files
	src/main.hpp
	src/gl_core/gl_core_3_3.h
	)


find_path(GLFW_INCLUDE GLFW/glfw3.h PATHS NO_DEFAULT_PATH DOC "GLFW include path")
find_path(GLFW_LIB glfw3.dll PATHS NO_DEFAULT_PATH DOC "GLFW lib path")
include_directories(${GLFW_INCLUDE})
include_directories(${GLFW_LIB})
find_library(GLFW_LIBRARY "glfw3" HINTS ${GLFW_LIB})

add_definitions(-DWINDOWS -lglfw3 -lgdi32 -lopengl32 -limm32)

add_executable(${project_name} ${header_files} ${source_files})
target_link_libraries(${project_name} ${GLFW_LIBRARY} opengl32)
