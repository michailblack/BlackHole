cmake_minimum_required(VERSION 3.25)

project(Glad)

add_library(${PROJECT_NAME}
	${PROJECT_SOURCE_DIR}/src/glad.c
	${PROJECT_SOURCE_DIR}/include/glad/glad.h
	${PROJECT_SOURCE_DIR}/include/KHR/khrplatform.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/include
)