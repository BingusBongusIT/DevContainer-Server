cmake_minimum_required(VERSION 3.20)
project(CC-Server)

set(CMAKE_CXX_STANDARD 23)

file(GLOB HEADER_FILES "sources/header/*.hpp")
file(GLOB SOURCE_FILES "sources/src/*.cpp")

set(CMAKE_GENERATOR_PLATFORM x64)
set(LIBRARIES ${LIBRARIES} uuid)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
