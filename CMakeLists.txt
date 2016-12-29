cmake_minimum_required(VERSION 2.8)

project(CORERobotLib)
set(TEAM_NUMBER 2062)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Robot target
# For quick list run
# find src -iname "*.cpp"
# in project root
set(SOURCE_FILES main.cpp
        src/COREAuto.cpp
        src/COREDrive.cpp
        src/COREHardware/COREEncoder.cpp
        src/COREHardware/COREJoystick.cpp
        src/COREHardware/COREMotor.cpp
        src/COREHardware/CORETimer.cpp
        src/COREHardware/CORESensor.cpp
        src/COREHardware.cpp
        src/CORELog.cpp
        src/COREPID.cpp
        src/CORERobot.cpp
        src/COREScheduler.cpp)

add_subdirectory(ARM)

include_directories(src/Fake_WPILib)
include_directories(src)
remove_definitions(__arm__)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})