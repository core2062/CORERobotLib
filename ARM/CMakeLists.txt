#[[# Robot target
# For quick list run
# find src -iname "*.cpp"
# in project root
set(SOURCE_FILES ../main.cpp
        ../src/COREAuto.cpp
        ../src/COREDrive.cpp
        ../src/COREHardware/COREEncoder.cpp
        ../src/COREHardware/COREJoystick.cpp
        ../src/COREHardware/COREMotor.cpp
        ../src/COREHardware/CORETimer.cpp
        ../src/COREHardware/CORESensor.cpp
        ../src/COREHardware.cpp
        ../src/CORELog.cpp
        ../src/COREPID.cpp
        ../src/CORERobot.cpp
        ../src/COREScheduler.cpp
        ../src/COREDrive/CORESwerve.cpp)

set(CMAKE_PREFIX_PATH )

set(PROJECT_NAME "ARM_${PROJECT_NAME}")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set(CMAKE_C_COMPILER /usr/bin/arm-frc-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-frc-linux-gnueabi-g++)
find_library(TALONSRXLIB TalonSRXLib HINTS ../../3rdparty/CANTalon/lib)
find_library(NAVX_FRC_CPP navx_frc_cpp HINTS ../../3rdparty/NavX/lib)
find_library(WPILIB wpi HINTS ~/wpilib/cpp/current/lib)
target_link_libraries(${PROJECT_NAME} ${TALONSRXLIB})
target_link_libraries(${PROJECT_NAME} ${NAVX_FRC_CPP})
target_link_libraries(${PROJECT_NAME} ${WPILIB})
include_directories(../../3rdparty/CANTalon/include)
include_directories(../../3rdparty/NavX/include)
include_directories(/home/akemperfi/wpilib/cpp/current/include)

include_directories(../src)
add_definitions(-D__arm__)

# Deploy target
find_file(ROBOTCOMMAND ant/robotCommand /usr/share/wpilib)
add_custom_target(deploy
        COMMAND sh -c "../../ARM/deploy.sh ${TEAM_NUMBER} $<TARGET_FILE:${PROJECT_NAME}> ${ROBOTCOMMAND}"
        DEPENDS ${PROJECT_NAME})
set_target_properties(deploy PROPERTIES EXCLUDE_FROM_ALL TRUE)]]

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(WPILIB_HOME $ENV{HOME}/wpilib/cpp/current)
set(PROJECT_NAME "ARM_${PROJECT_NAME}")

set(CMAKE_C_COMPILER /usr/bin/arm-frc-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-frc-linux-gnueabi-g++)

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
set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/src/COREAuto.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDrive.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/COREEncoder.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/COREJoystick.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/COREMotor.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/CORETimer.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware/CORESensor.cpp
        ${PROJECT_SOURCE_DIR}/src/COREHardware.cpp
        ${PROJECT_SOURCE_DIR}/src/CORELog.cpp
        ${PROJECT_SOURCE_DIR}/src/COREPID.cpp
        ${PROJECT_SOURCE_DIR}/src/CORERobot.cpp
        ${PROJECT_SOURCE_DIR}/src/COREScheduler.cpp
        ${PROJECT_SOURCE_DIR}/src/COREDrive/CORESwerve.cpp)

# Deploy target

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} libwpi.so libnavx_frc_cpp.a libTalonSRXLib.a)
