#[[
cmake_minimum_required(VERSION 2.8)

project(CORERobotLib)
set(TEAM_NUMBER 2062)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")

# Robot target
# For quick list run
# find src -iname "*.cpp"
# in project root
set(SOURCE_FILES main.cpp
        src/COREAuton.cpp
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
        src/COREScheduler.cpp src/COREDrive/CORESwerve.cpp src/Fake_WPILib/WPILib.h src/Fake_WPILib/WPILib.h)



include_directories(src/Fake_WPILib)
include_directories(src)
remove_definitions(__arm__)
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})]]

cmake_minimum_required(VERSION 2.8)

project(CORERobotLib)

# Team number
set(TEAM_NUMBER 2062)

# Robot target
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")

add_subdirectory(ARM)

set(WPILIB_HOME $ENV{HOME}/wpilib/cpp/current)
set(${PROJECT_NAME} ARM_${PROJECT_NAME})

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

#include_directories(${PROJECT_SOURCE_DIR}/3rdparty/CANTalon/include)
#include_directories(${PROJECT_SOURCE_DIR}/3rdparty/NavX/include)

include_directories(${PROJECT_SOURCE_DIR}/src/Fake_WPILib)
#include_directories(${WPILIB_HOME}/include)

# For quick list run
# find src -iname "*.cpp"
# in project root
set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/src/COREAuton.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDrive.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/COREEncoder.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/COREJoystick.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/COREMotor.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/CORETimer.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/CORESensor.cpp
        ${PROJECT_SOURCE_DIR}/src/CORELog.cpp
        ${PROJECT_SOURCE_DIR}/src/COREPID.cpp
        ${PROJECT_SOURCE_DIR}/src/CORERobot.cpp
        ${PROJECT_SOURCE_DIR}/src/COREScheduler.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDrive/CORESwerve.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDataLog.cpp
        ${PROJECT_SOURCE_DIR}/src/COREConstant.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDrive/COREEtherDrive.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware.cpp
        ${PROJECT_SOURCE_DIR}/src/Fake_WPILib/WPILib.cpp)

# Deploy target

include_directories(src)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME})


#add_frc_deploy(deploy ${TEAM_NUMBER} $<TARGET_FILE:${PROJECT_NAME}>)
