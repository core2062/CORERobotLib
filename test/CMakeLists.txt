cmake_minimum_required(VERSION 3.6)

project(CORERobotLib_Test)

set(CMAKE_CXX_STANDARD 14)

### Project Setup
set(PROJECT_SOURCE_DIR "../")
add_definitions(-DFAKE)

### Include directories
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/CORESimulation)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Catch/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/seasocks/src/main/c)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/json/src)

#### Seasocks
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/seasocks)
set(UNITTESTS OFF CACHE BOOL "Build unittests." FORCE)
set(DEFLATE_SUPPORT OFF CACHE BOOL "Include support for deflate (requires zlib)." FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/seasocks ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/seasocks)

### Add source files
add_subdirectory(${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)
set(SOURCE_FILES
        $<TARGET_OBJECTS:CORELogging>
        $<TARGET_OBJECTS:COREControl>
        $<TARGET_OBJECTS:COREDashboard>
        $<TARGET_OBJECTS:COREUtilities>
        $<TARGET_OBJECTS:COREFramework>
        $<TARGET_OBJECTS:CORESimulation>
        $<TARGET_OBJECTS:COREData>)

## CORESimRobot
add_executable(Main main.cpp ${SOURCE_FILES})
target_link_libraries(Main seasocks "${ZLIB_LIBRARIES}")


#### Testing
enable_testing()

add_executable(${PROJECT_NAME}
        $<TARGET_OBJECTS:CORELogging>
        $<TARGET_OBJECTS:COREDashboard>
        $<TARGET_OBJECTS:COREData>
        $<TARGET_OBJECTS:COREUtilities>
        $<TARGET_OBJECTS:COREControl>
        $<TARGET_OBJECTS:COREFramework>
        $<TARGET_OBJECTS:CORESimulation>
        CORELogging_Test.cpp
        COREUtilities_Test.cpp
        COREDashboard_Test.cpp
        COREControl_Test.cpp)
target_link_libraries(${PROJECT_NAME} seasocks)
add_test(${PROJECT_NAME} ${PROJECT_NAME})
