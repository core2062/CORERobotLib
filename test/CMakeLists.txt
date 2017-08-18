cmake_minimum_required(VERSION 3.6)

project(CORERobotLib_Test)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-DSIM)

set(PROJECT_SOURCE_DIR "../")

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/seasocks/src/main/c)
include_directories(${PROJECT_SOURCE_DIR}/src/CORESimulation)

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/seasocks)
set(UNITTESTS OFF CACHE BOOL "Build unittests." FORCE)
set(DEFLATE_SUPPORT OFF CACHE BOOL "Include support for deflate (requires zlib)." FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/seasocks ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/seasocks)

add_subdirectory(${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/googletest/googletest ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/googletest/googletest)

include_directories(${gtest_SOURCE_DIR}/include)

add_executable(Main main.cpp $<TARGET_OBJECTS:COREDashboard> $<TARGET_OBJECTS:COREUtilities> $<TARGET_OBJECTS:CORELogging>)
target_link_libraries(Main seasocks "${ZLIB_LIBRARIES}")

enable_testing()

add_executable(CORELog_Tests CORELog_Test.cpp $<TARGET_OBJECTS:CORELogging>)
target_link_libraries(CORELog_Tests gtest gtest_main)
add_test(CORELog_Tests CORELog_Tests)

add_executable(COREUtilities_Test COREUtilities_Test.cpp $<TARGET_OBJECTS:COREUtilities> $<TARGET_OBJECTS:CORESimulation>)
target_link_libraries(COREUtilities_Test gtest gtest_main)
add_test(COREUtilities_Test COREUtilities_Test)

add_executable(COREDashboard_Test COREDashboard_Test.cpp $<TARGET_OBJECTS:COREDashboard> $<TARGET_OBJECTS:COREUtilities> $<TARGET_OBJECTS:CORELogging>)
target_link_libraries(COREDashboard_Test gtest gtest_main seasocks "${ZLIB_LIBRARIES}")

add_test(COREDashboard_Test COREDashboard_Test)