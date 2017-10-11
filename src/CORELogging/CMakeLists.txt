cmake_minimum_required(VERSION 3.0)

set(NAME CORELogging)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        COREDataLog.cpp
        CORELog.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/wpilib/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/CTRE/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/navx-mxp/include)

add_library(${NAME} OBJECT ${SOURCE_FILES})
