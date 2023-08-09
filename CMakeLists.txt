cmake_minimum_required(VERSION 3.5...3.27)

project(BlackHole-Engine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

add_compile_definitions(
	$<$<CONFIG:Debug>:BH_DEBUG>
	$<$<CONFIG:Debug>:BH_ENABLE_ASSERTS>
	$<$<CONFIG:Release>:BH_RELEASE>
)

add_subdirectory(BlackHole)
add_subdirectory(BlackHole-Editor)