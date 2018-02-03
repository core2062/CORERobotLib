#cmake_minimum_required(VERSION 3.3)
#project(CORERobotLib)
#set(CMAKE_CXX_STANDARD 14)
#
#file(GLOB_RECURSE SOURCES "src/*.*")
#file(GLOB_RECURSE INCLUDES "src/*.*")
#include_directories(src)
#
#set(WPILIB_HOME $ENV{HOME}/wpilib/cpp/current)
#set(WPILIB_USER $ENV{HOME}/wpilib/user/cpp)
#
#file(GLOB_RECURSE DEP_INCLUDES
#"${WPILIB_HOME}/include/*.*"
#"${WPILIB_USER}/include/*.*"
#"3rdparty/navx-mxp/include/*.*"
#"3rdparty/json/src/*.*"
#"3rdparty/seasocks/src/main/c/*.*")
#
#include_directories(${WPILIB_HOME}/include/)
#include_directories(${WPILIB_HOME}/reflib/)
#include_directories(${WPILIB_USER}/include/)
#include_directories(${WPILIB_USER}/lib/)
#include_directories(3rdparty/navx-mxp/include)
#include_directories(3rdparty/navx-mxp/Linux)
#include_directories(3rdparty/json/src)
#include_directories(3rdparty/seasocks/src/main/c)
#
#set(ALL_INCLUDES ${INCLUDES} ${DEP_INCLUDES})
#
#add_executable(libCORERobotLib.so ${SOURCES} ${ALL_INCLUDES})
#
#add_custom_target(libCORERobotLib.so_build gradlew.bat build WORKING_DIRECTORY ../ SOURCES ${SOURCES} ${ALL_INCLUDES})
#add_custom_target(libCORERobotLib.so_deploy gradlew.bat deploy WORKING_DIRECTORY ../ SOURCES ${SOURCES} ${ALL_INCLUDES})
