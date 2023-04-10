cmake_minimum_required(VERSION 3.20)
project(CC-Server)

set(CMAKE_CXX_STANDARD 23)

file(GLOB HEADER_FILES "sources/header/*.hpp")
file(GLOB SOURCE_FILES "sources/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
