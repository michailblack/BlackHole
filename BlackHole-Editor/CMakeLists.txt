cmake_minimum_required(VERSION 3.5...3.27)

project(BlackHole-Editor CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)

file(GLOB_RECURSE BH_EDITOR_SRC
    src/BlackHole-Editor/*.cpp
    src/BlackHoleEditorApp.cpp
)

add_executable(${PROJECT_NAME} ${BH_EDITOR_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC
    src
)

include_directories(
    ${CMAKE_SOURCE_DIR}/BlackHole/src
    ${CMAKE_SOURCE_DIR}/BlackHole/vendor
)

target_link_libraries(${PROJECT_NAME} BlackHole)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)