cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        CANTalon.cpp
        WPILib.cpp
        CORESimRobot.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(CORESimulation OBJECT ${SOURCE_FILES})