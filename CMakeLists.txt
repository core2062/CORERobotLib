cmake_minimum_required(VERSION 3.6)

project(CORERobotLib)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(CORERobotLib)
add_subdirectory(3rdparty/googletest/googletest)

set(SOURCE_FILES
        $<TARGET_OBJECTS:CORELogging>
        $<TARGET_OBJECTS:COREControl>
        $<TARGET_OBJECTS:COREUtilities>
        $<TARGET_OBJECTS:COREHardware>
        $<TARGET_OBJECTS:COREFramework>
        $<TARGET_OBJECTS:COREDrive>
        $<TARGET_OBJECTS:WaypointFollower>)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL arm)
    find_library(WPILIB_LOCATION NAMES wpilibc PATHS ${PROJECT_SOURCE_DIR}/3rdparty/wpilib/Linux/arm/)
    find_library(CTRE_LOCATION NAMES CTRLib PATHS ${PROJECT_SOURCE_DIR}/3rdparty/CTRE/Linux/arm/)
    find_library(NAVX-MXP_LOCATION NAMES navx_frc_cpp PATHS ${PROJECT_SOURCE_DIR}/3rdparty/navx-mxp/Linux/arm/)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/wpilib/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/CTRE/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/navx-mxp/include)
else()
    message(NOT YET IMPLEMENTED)
endif()

target_link_libraries(${PROJECT_NAME} ${WPILIB_LOCATION} ${CTRE_LOCATION} ${NAVX-MXP_LOCATION})

include_directories(${gtest_SOURCE_DIR}/include)
