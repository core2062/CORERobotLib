#include "gtest/gtest.h"
#include "CORESimulation/mock_CANTalon.cpp"

using namespace testing;

TEST(Thisisatest, numbers) {
    CANTalon talon(1);
    EXPECT_CALL(talon, Set(100));
    talon.Set(100);
}