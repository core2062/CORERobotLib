cmake_minimum_required(VERSION 3.6)

project(CORERobotLib_Test)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_SOURCE_DIR "../")

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/CORESimulation)

add_subdirectory(${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/googletest/googletest ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/googletest/googletest)

#set(SOURCE_FILES
##        $<TARGET_OBJECTS:CORELogging>
#        $<TARGET_OBJECTS:COREControl>
#        $<TARGET_OBJECTS:COREUtilities>
#        $<TARGET_OBJECTS:COREHardware>
#        $<TARGET_OBJECTS:COREFramework>
#        $<TARGET_OBJECTS:COREDrive>
#        $<TARGET_OBJECTS:WaypointFollower>
#        $<TARGET_OBJECTS:CORESimulation>)
#
#add_library(${PROJECT_NAME} ${SOURCE_FILES})

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include)


add_executable(CORELog_Tests CORELog_Test.cpp $<TARGET_OBJECTS:CORELogging>)
target_link_libraries(CORELog_Tests gtest gtest_main)
add_test(CORELog_Tests CORELog_Tests)

add_executable(COREUtilities_Test COREUtilities_Test.cpp $<TARGET_OBJECTS:COREUtilities> $<TARGET_OBJECTS:CORESimulation>)
target_link_libraries(COREUtilities_Test gtest gtest_main)

add_test(COREUtilities_Test COREUtilities_Test)


#set(TEST_SOURCES
#../../test/CORELog_test.cpp)
#add_executable(${NAME}_Tests ${TEST_SOURCES})
#target_link_libraries(${NAME}_Tests gtest gtest_main)
#
#add_test(${NAME}_Tests CORELogging_Tests)