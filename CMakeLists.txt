cmake_minimum_required(VERSION 3.25)

project(BlackHole CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

file(GLOB_RECURSE BH_SRC
    src/BlackHole/*.cpp
    src/BlackHole/*.h
    src/EntryPoint.cpp
)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/Glad)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/ImGui)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/stb_image)

add_compile_definitions(
	$<$<CONFIG:Debug>:BH_DEBUG>
	$<$<CONFIG:Debug>:BH_ENABLE_ASSERTS>
	$<$<CONFIG:Release>:BH_RELEASE>
	GLFW_INCLUDE_NONE)

add_executable(${CMAKE_PROJECT_NAME} ${BH_SRC})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    src
    vendor/spdlog/include
    vendor/Glad/include
    vendor/GLFW/include
	vendor/glm
    vendor/assimp/include
    vendor/stb_image
)

target_link_libraries(${CMAKE_PROJECT_NAME} spdlog)
target_link_libraries(${CMAKE_PROJECT_NAME} Glad)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} imgui)
target_link_libraries(${CMAKE_PROJECT_NAME} assimp)
target_link_libraries(${CMAKE_PROJECT_NAME} stb_image)

target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE src/bhpch.h)