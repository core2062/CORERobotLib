cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        WPILib312312.cpp
        #CORESimRobot.cpp
        CANTalon.h
        Joystick.h)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/wpilib/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/CTRE/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/navx-mxp/include)

add_library(CORESimulation OBJECT ${SOURCE_FILES})
