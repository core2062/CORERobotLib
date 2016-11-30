# Robot target
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
        ../src/COREHardware.cpp
        ../src/CORELog.cpp
        ../src/COREPID.cpp
        ../src/CORERobot.cpp
        ../src/COREScheduler.cpp)

set(PROJECT_NAME "ARM_${PROJECT_NAME}")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set(CMAKE_C_COMPILER /usr/bin/arm-frc-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-frc-linux-gnueabi-g++)
find_library(WPILIB wpi HINTS ~/wpilib/cpp/current/lib)
target_link_libraries(${PROJECT_NAME} ${WPILIB})
include_directories(/home/akemperfi/wpilib/cpp/current/include)
#add_definitions(-D__arm__)

# Deploy target
find_file(ROBOTCOMMAND ant/robotCommand /usr/share/wpilib)
add_custom_target(deploy
        COMMAND sh -c "./deploy.sh ${TEAM_NUMBER} $<TARGET_FILE:${PROJECT_NAME}> ${ROBOTCOMMAND}"
        DEPENDS ${PROJECT_NAME})
set_target_properties(deploy PROPERTIES EXCLUDE_FROM_ALL TRUE)