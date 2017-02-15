set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g3 -Wall")
set(CMAKE_SYSTEM_NAME Linux)

if(WIN32)
    set(WPILIB_HOME C:/Users/Andrew/wpilib/cpp/current)
    #TODO: This needs to be changed to work for everyone
else()
    set(WPILIB_HOME $ENV{HOME}/wpilib/cpp/current)
endif()

set(PROJECT_NAME "ARM_${PROJECT_NAME}")

if(WIN32)
    set(CMAKE_C_COMPILER C:/frc/bin/arm-frc-linux-gnueabi-gcc)
    set(CMAKE_CXX_COMPILER C:/frc/bin/arm-frc-linux-gnueabi-g++)
else()
    set(CMAKE_C_COMPILER /usr/bin/arm-frc-linux-gnueabi-gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/arm-frc-linux-gnueabi-g++)
endif()

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/CANTalon/include)
link_directories(${PROJECT_SOURCE_DIR}/3rdparty/CANTalon/lib)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/NavX/include)
link_directories(${PROJECT_SOURCE_DIR}/3rdparty/NavX/lib)

include_directories(${WPILIB_HOME}/include)
link_directories(${WPILIB_HOME}/lib)

include_directories(${PROJECT_SOURCE_DIR})

# For quick list run
# find src -iname "*.cpp"
# in project root
set(SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/src/COREAuton.cpp
        ${PROJECT_SOURCE_DIR}/src/COREConstant.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDataLog.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDrive/COREEtherDrive.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDrive/CORESwerveDrive.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDrive.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/COREEncoder.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/COREJoystick.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/COREMotor.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/CORESensor.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/CORETimer.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware.cpp
        ${PROJECT_SOURCE_DIR}/src/CORELog.cpp
        ${PROJECT_SOURCE_DIR}/src/COREPID.cpp
        ${PROJECT_SOURCE_DIR}/src/CORERobot.cpp
        ${PROJECT_SOURCE_DIR}/src/COREScheduler.cpp
        ${PROJECT_SOURCE_DIR}/src/COREController/COREMotionProfile.cpp
        ${PROJECT_SOURCE_DIR}/src/COREController.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} libwpi.so libnavx_frc_cpp.a libCTRLib.a)