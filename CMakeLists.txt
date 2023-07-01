cmake_minimum_required(VERSION 3.26.4)

MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
	IF(MSVC)
		GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
		SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
		SET(Sources ${${SourcesVar}})

		SET_SOURCE_FILES_PROPERTIES(
			${PrecompiledSource}	PROPERTIES
			COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
			OBJECT_OUTPUTS "${PrecompiledBinary}"
		)
		
		SET_SOURCE_FILES_PROPERTIES(
			${Sources} PROPERTIES
			COMPILE_FLAGS "/Yu\"${PrecompiledBinary}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
			OBJECT_DEPENDS "${PrecompiledBinary}"
		)
	ENDIF(MSVC)
	
	# Add precompiled header to SourcesVar
	LIST(APPEND ${SourcesVar} ${PrecompiledSource})
ENDMACRO()

project(BlackHole CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(BH_SRC_PATH src/BlackHole)

file(GLOB_RECURSE BH_SRC
    ${BH_SRC_PATH}/*.cpp
    ${BH_SRC_PATH}/*.h
    src/BlackHoleApp.cpp
)

file(GLOB_RECURSE SRC_CORE
    ${BH_SRC_PATH}/Core/*.cpp
    ${BH_SRC_PATH}/Core/*.h
)

file(GLOB_RECURSE SRC_EVENTS
    ${BH_SRC_PATH}/Events/*.cpp
    ${BH_SRC_PATH}/Events/*.h
)

file(GLOB_RECURSE SRC_IMGUI
    ${BH_SRC_PATH}/ImGui/*.cpp
    ${BH_SRC_PATH}/ImGui/*.h
)

source_group(BlackHole/Core FILES ${SRC_CORE})
source_group(BlackHole/Events FILES ${SRC_EVENTS})
source_group(BlackHole/ImGui FILES ${SRC_IMGUI})
source_group(BlackHole FILES src/BlackHoleApp.cpp src/bhpch.h src/bhpch.cpp)

ADD_MSVC_PRECOMPILED_HEADER(bhpch.h src/bhpch.cpp BH_SRC)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/Glad)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/glm)
#add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/ImGui)

add_compile_definitions(
	$<$<CONFIG:Debug>:BH_DEBUG>
	$<$<CONFIG:Debug>:BH_ENABLE_ASSERTS>
	$<$<CONFIG:Release>:BH_RELEASE>
	GLFW_INCLUDE_NONE)

add_executable(${CMAKE_PROJECT_NAME} src/BlackHoleApp.cpp src/bhpch.h src/bhpch.cpp ${SRC_CORE} ${SRC_EVENTS} ${SRC_IMGUI})
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    src
    vendor/spdlog/include
    vendor/Glad/include
    vendor/GLFW/include
	vendor/glm
)

target_link_libraries(${CMAKE_PROJECT_NAME} spdlog)
target_link_libraries(${CMAKE_PROJECT_NAME} Glad)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)