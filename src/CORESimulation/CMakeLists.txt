cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        CANTalon.cpp
        WPILib312312.cpp
        CORESimRobot.cpp
        CANTalon.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/wpilib/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/CTRE/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/navx-mxp/include)

add_library(CORESimulation OBJECT ${SOURCE_FILES})
