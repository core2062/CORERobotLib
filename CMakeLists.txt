cmake_minimum_required(VERSION 3.0)

project(CORERobotLib)

set(CMAKE_CXX_STANDARD 14)

### Set compiler to arm cross compiler toolchain
add_definitions(-DARM)

### Setup seasocks target
#[[set(UNITTESTS OFF CACHE BOOL "Build unittests." FORCE)
set(DEFLATE_SUPPORT OFF CACHE BOOL "Include support for deflate (requires zlib)." FORCE)
include_directories(3rdparty/seasocks)
add_subdirectory(3rdparty/seasocks)]]

### Find required libraries
find_library(WPILIB_LOCATION NAMES wpi PATHS ${PROJECT_SOURCE_DIR}/3rdparty/wpilib/Linux/arm/)
find_library(CTRE_LOCATION NAMES CTRLib PATHS ${PROJECT_SOURCE_DIR}/3rdparty/CTRE/Linux/arm/)
find_library(NAVX-MXP_LOCATION NAMES navx_frc_cpp PATHS ${PROJECT_SOURCE_DIR}/3rdparty/navx-mxp/Linux/arm/)
find_library(SEASOCKS_LOCATION NAMES seasocks PATHS ${PROJECT_SOURCE_DIR}/3rdparty/seasocks/Linux/arm/)

### 3rd party library interfaces
add_library(WPILIB INTERFACE)
target_include_directories(WPILIB INTERFACE ${PROJECT_SOURCE_DIR}/3rdparty/wpilib/include)
target_link_libraries(WPILIB INTERFACE ${WPILIB_LOCATION})

add_library(CTRE INTERFACE)
target_include_directories(CTRE INTERFACE ${PROJECT_SOURCE_DIR}/3rdparty/CTRE/include)
target_link_libraries(CTRE INTERFACE ${CTRE_LOCATION})

add_library(NAVX-MXP INTERFACE)
target_include_directories(NAVX-MXP INTERFACE ${PROJECT_SOURCE_DIR}/3rdparty/navx-mxp/include)
target_link_libraries(NAVX-MXP INTERFACE ${NAVX-MXP_LOCATION})

add_library(JSON INTERFACE)
target_include_directories(JSON INTERFACE ${PROJECT_SOURCE_DIR}/3rdparty/json/src)

add_library(seasocks INTERFACE)
target_include_directories(seasocks INTERFACE ${PROJECT_SOURCE_DIR}/3rdparty/seasocks/src/main/c)
target_link_libraries(seasocks INTERFACE ${SEASOCKS_LOCATION})

#target_include_directories(seasocks PUBLIC ${PROJECT_SOURCE_DIR}/3rdparty/seasocks/src/main/c)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/seasocks/src/main/c)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/json/src)

add_subdirectory(src)

### CORERobotLib target
set(SOURCE_FILES
        $<TARGET_OBJECTS:CORELogging>
        $<TARGET_OBJECTS:COREControl>
        $<TARGET_OBJECTS:COREDashboard>
        $<TARGET_OBJECTS:COREData>
        $<TARGET_OBJECTS:COREUtilities>
        $<TARGET_OBJECTS:COREHardware>
        $<TARGET_OBJECTS:COREFramework>
        $<TARGET_OBJECTS:COREDrive>
        $<TARGET_OBJECTS:WaypointFollower>)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} CTRE NAVX-MXP WPILIB seasocks JSON)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)
