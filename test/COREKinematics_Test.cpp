#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "COREKinematics/TankKinematics.h"

using namespace testing;

TEST(COREKinematics, Test) {
    std::cout << "X: " << TankKinematics::forwardKinematics(1, 0.5).dx << " Y: " << TankKinematics::forwardKinematics(1, 0.5).dy << " Theta: " << TankKinematics::forwardKinematics(1, 0.5).dtheta << std::endl;
}
