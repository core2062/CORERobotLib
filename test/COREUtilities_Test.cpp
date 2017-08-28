#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "COREUtilities/CORETimer.h"
#include "CORESimulation/WPILib.h"

using namespace CORE;

TEST_CASE("StartStop") {
    CORETimer timer;
    timer.Start();
    Wait(0.01);
    REQUIRE(abs(timer.Get() - 0.01) < 0.01);
    timer.Stop();
    Wait(0.01);
    REQUIRE(abs(timer.Get() - 0.01) < 0.01);
}

TEST_CASE("1==1") {
    REQUIRE(1==1);
}

//TEST(CORELog, CORELogInit) {
//    CORE::CORELog::robotInit();
//    ASSERT_NO_THROW(CORE::CORELog::getName());
//}