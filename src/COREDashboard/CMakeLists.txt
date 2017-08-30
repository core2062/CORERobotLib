cmake_minimum_required(VERSION 3.6)

set(NAME COREDashboard)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        COREDashboard.cpp COREConnectionHandler.cpp COREConnectionHandler.h COREDriverstation.cpp COREDriverstation.h)

include_directories(${PROJECT_SOURCE_DIR}/src)

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/wpilib/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/CTRE/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/navx-mxp/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/seasocks/src/main/c)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/json/src)

add_library(${NAME} OBJECT ${SOURCE_FILES})
