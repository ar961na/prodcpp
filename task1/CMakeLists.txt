cmake_minimum_required(VERSION 3.27)
project(task1)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        main.cpp
        group.cpp
        user.cpp
        command.cpp)

set(HEADERS
        app.h
        group.h
        user.h
        command.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
