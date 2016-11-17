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
        src/COREHardware.cpp
        src/CORELog.cpp
        src/COREPID.cpp
        src/CORERobot.cpp
        src/CORESubsystemsManager.cpp)

if(CMAKE_BUILD_TYPE STREQUAL Release)
    set(PROJECT_NAME "ARM_${PROJECT_NAME}")
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    set(CMAKE_C_COMPILER /usr/bin/arm-frc-linux-gnueabi-gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/arm-frc-linux-gnueabi-g++)
    find_library(WPILIB wpi HINTS ~/wpilib/cpp/current/lib)
    target_link_libraries(${PROJECT_NAME} ${WPILIB})
    include_directories(/home/akemperfi/wpilib/cpp/current/include)
    add_definitions(-D__arm__)
else()
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    set(CMAKE_C_COMPILER /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif()

# Deploy target
find_file(ROBOTCOMMAND ant/robotCommand /usr/share/wpilib)
add_custom_target(deploy
        COMMAND sh -c "./deploy.sh ${TEAM_NUMBER} $<TARGET_FILE:${PROJECT_NAME}> ${ROBOTCOMMAND}"
        DEPENDS ${PROJECT_NAME})
set_target_properties(deploy PROPERTIES EXCLUDE_FROM_ALL TRUE)