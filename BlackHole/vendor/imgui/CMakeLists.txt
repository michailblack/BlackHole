cmake_minimum_required(VERSION 3.25)

project(imgui CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

file(GLOB IMGUI_SRC
    *.cpp
    *.h
)

set(IMGUI_PLATFORM
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_glfw.h
)

set(IMGUI_RENDERER
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_opengl3.h
)

add_library(${PROJECT_NAME} ${IMGUI_SRC} ${IMGUI_PLATFORM} ${IMGUI_RENDERER})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
    ../GLFW/include
)
