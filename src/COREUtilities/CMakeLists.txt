cmake_minimum_required(VERSION 3.6)

set(NAME COREUtilities)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        COREConstant.cpp
        CORETimer.cpp
        CORENamedObject.cpp
        Rotation2d.cpp
        Translation2d.cpp
        Position2d.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/wpilib/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/CTRE/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/navx-mxp/include)

add_library(${NAME} OBJECT ${SOURCE_FILES})

#set(TEST_SOURCES
#        )
#add_executable(${NAME}_Tests ${TEST_SOURCES})
#target_link_libraries(${NAME}_Tests gtest gtest_main)
#
#add_test(${NAME}_Tests CORELogging_Tests)