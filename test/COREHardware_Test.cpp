#include "gtest/gtest.h"
#include "gmock/gmock.h"

#define private public

#include "COREHardware/COREJoystick.h"
#include "COREUtilities/COREDriverstation.h"

using namespace CORE;
using ::testing::Return;

TEST(COREJoystick, joystickXY) {
    COREDriverstation::overrideRobotMode(COREDriverstation::TELEOP);
    COREJoystick joystick(0);
    EXPECT_CALL(joystick.m_joystick, GetRawAxis(COREJoystick::LEFT_STICK_X)).WillOnce(Return(0.1));
    EXPECT_CALL(joystick.m_joystick, GetRawAxis(COREJoystick::LEFT_STICK_Y)).WillOnce(Return(0.2));
    EXPECT_CALL(joystick.m_joystick, GetRawAxis(COREJoystick::RIGHT_STICK_X)).WillOnce(Return(0.3));
    EXPECT_CALL(joystick.m_joystick, GetRawAxis(COREJoystick::RIGHT_STICK_Y)).WillOnce(Return(0.4));
    joystick.registerAxis(COREJoystick::LEFT_STICK_X);
    joystick.registerAxis(COREJoystick::LEFT_STICK_Y);
    joystick.registerAxis(COREJoystick::RIGHT_STICK_X);
    joystick.registerAxis(COREJoystick::RIGHT_STICK_Y);
    EXPECT_EQ(joystick.getAxis(COREJoystick::LEFT_STICK_X), 0.1);
    EXPECT_EQ(joystick.getAxis(COREJoystick::LEFT_STICK_Y), 0.2);
    EXPECT_EQ(joystick.getAxis(COREJoystick::RIGHT_STICK_X), 0.3);
    EXPECT_EQ(joystick.getAxis(COREJoystick::RIGHT_STICK_Y), 0.4);
    EXPECT_CALL(joystick.m_joystick, GetRawAxis(COREJoystick::LEFT_STICK_X)).WillOnce(Return(0.75));
    EXPECT_CALL(joystick.m_joystick, GetRawAxis(COREJoystick::LEFT_STICK_Y)).WillOnce(Return(0.76));
    EXPECT_CALL(joystick.m_joystick, GetRawAxis(COREJoystick::RIGHT_STICK_X)).WillOnce(Return(0.77));
    EXPECT_CALL(joystick.m_joystick, GetRawAxis(COREJoystick::RIGHT_STICK_Y)).WillOnce(Return(0.78));
    joystick.preLoopTask();
    EXPECT_EQ(joystick.getAxis(COREJoystick::LEFT_STICK_X), 0.75);
    EXPECT_EQ(joystick.getAxis(COREJoystick::LEFT_STICK_Y), 0.76);
    EXPECT_EQ(joystick.getAxis(COREJoystick::RIGHT_STICK_X), 0.77);
    EXPECT_EQ(joystick.getAxis(COREJoystick::RIGHT_STICK_Y), 0.78);

    COREDriverstation::overrideRobotMode(COREDriverstation::AUTON);
    EXPECT_CALL(joystick.m_joystick, GetRawAxis(COREJoystick::LEFT_STICK_X)).WillRepeatedly(Return(0.1));
    joystick.preLoopTask();
    EXPECT_EQ(joystick.getAxis(COREJoystick::LEFT_STICK_X), 0);

    COREDriverstation::overrideRobotMode(COREDriverstation::DISABLE);
    joystick.preLoopTask();
    EXPECT_EQ(joystick.getAxis(COREJoystick::LEFT_STICK_X), 0);
}
