#include "gtest/gtest.h"

#include "COREUtilities/CORETimer.h"

#include "CORESimulation/WPILib.h"

using namespace CORE;

TEST(CORETimer, StartStop) {
    CORETimer timer;
    timer.Start();
    Wait(0.1);
    EXPECT_NEAR(timer.Get(), 0.1, 0.02);
    timer.Stop();
    Wait(0.1);
    EXPECT_NEAR(timer.Get(), 0.1, 0.02);
}

TEST(CORETimer, Test) {
    EXPECT_EQ(1, 1);
}

//TEST(CORELog, CORELogInit) {
//    CORE::CORELog::robotInit();
//    ASSERT_NO_THROW(CORE::CORELog::getName());
//}