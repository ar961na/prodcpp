cmake_minimum_required(VERSION 3.5)

project(task7)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


