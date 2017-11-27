#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "COREDrive/CORESwerveDrive.h"
#include "CORESimulation/ctrlib/CANTalon.h"

using namespace testing;
using namespace CORE;

TEST(CORESwerve, XYMove) {
    CANTalon steerMotor(1);
    CANTalon driveMotor(1);
    auto frontRight = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto frontLeft = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto backRight = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto backLeft = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);

    auto swerve = new CORESwerve(1, 1, frontRight, frontLeft, backRight, backLeft);

    EXPECT_CALL(steerMotor, GetAnalogInRaw()).WillRepeatedly(Return(0));
    EXPECT_CALL(driveMotor, Get()).WillRepeatedly(Return(0));

    //Forwards
    swerve->calculate(1, 0, 0);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleSpeed, 1);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleSpeed, 1);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleSpeed, 1);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleSpeed, 1);

    //Right
    swerve->calculate(0, 0.75, 0);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleAngle, 90);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleSpeed, 0.75);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleAngle, 90);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleSpeed, 0.75);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleAngle, 90);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleSpeed, 0.75);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleAngle, 90);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleSpeed, 0.75);
}

TEST(CORESwerve, Optimization) {
    CANTalon steerMotor(1);
    CANTalon driveMotor(1);
    auto frontRight = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto frontLeft = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto backRight = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto backLeft = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);

    auto swerve = new CORESwerve(1, 1, frontRight, frontLeft, backRight, backLeft);

    EXPECT_CALL(steerMotor, GetAnalogInRaw()).WillRepeatedly(Return(0));
    EXPECT_CALL(driveMotor, Get()).WillRepeatedly(Return(0));

    //Forward
    swerve->calculate(0.25, 0, 0);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleSpeed, 0.25);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleSpeed, 0.25);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleSpeed, 0.25);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleSpeed, 0.25);

    //Backwards
    swerve->calculate(-0.5, 0, 0);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleSpeed, -0.5);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleSpeed, -0.5);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleSpeed, -0.5);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleAngle, 0);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleSpeed, -0.5);
}


TEST(CORESwerve, Rotation) {
    CANTalon steerMotor(1);
    CANTalon driveMotor(1);
    auto frontRight = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto frontLeft = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto backRight = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto backLeft = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);

    auto swerve = new CORESwerve(1, 1, frontRight, frontLeft, backRight, backLeft);

    EXPECT_CALL(steerMotor, GetAnalogInRaw()).WillRepeatedly(Return(0));
    EXPECT_CALL(driveMotor, Get()).WillRepeatedly(Return(0));

    //Clockwise
    swerve->calculate(0, 0, -1);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleAngle, 315);
    EXPECT_NEAR(swerve->rightFrontModuleSpeed, 1, 0.01);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleAngle, 45);
    EXPECT_NEAR(swerve->leftFrontModuleSpeed, -1, 0.01);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleAngle, 45);
    EXPECT_NEAR(swerve->rightBackModuleSpeed, 1, 0.01);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleAngle, 315);
    EXPECT_NEAR(swerve->leftBackModuleSpeed, -1, 0.01);

    //Half Speed Clockwise
    swerve->calculate(0, 0, 0.5);
    EXPECT_DOUBLE_EQ(swerve->rightFrontModuleAngle, 315);
    EXPECT_NEAR(swerve->rightFrontModuleSpeed, -0.5, 0.01);
    EXPECT_DOUBLE_EQ(swerve->leftFrontModuleAngle, 45);
    EXPECT_NEAR(swerve->leftFrontModuleSpeed, 0.5, 0.01);
    EXPECT_DOUBLE_EQ(swerve->rightBackModuleAngle, 45);
    EXPECT_NEAR(swerve->rightBackModuleSpeed, -0.5, 0.01);
    EXPECT_DOUBLE_EQ(swerve->leftBackModuleAngle, 315);
    EXPECT_NEAR(swerve->leftBackModuleSpeed, 0.5, 0.01);
}

TEST(CORESwerve, AnglePID) {
    CANTalon steerMotor(1);
    CANTalon driveMotor(1);
    auto frontRight = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto frontLeft = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto backRight = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);
    auto backLeft = new CORESwerve::SwerveModule(&driveMotor, &steerMotor);

    auto swerve = new CORESwerve(1, 1, frontRight, frontLeft, backRight, backLeft);
    swerve->setSteerPID(1, 0, 0);

    EXPECT_CALL(steerMotor, GetAnalogInRaw()).WillRepeatedly(Return(0));
    EXPECT_CALL(driveMotor, Get()).WillRepeatedly(Return(0));

    EXPECT_CALL(driveMotor, Set(1)).Times(4);
    EXPECT_CALL(steerMotor, Set(0)).Times(4);
    swerve->calculate(1, 0, 0);
    swerve->update(0.01);

    EXPECT_CALL(driveMotor, Set(1)).Times(4);
    EXPECT_CALL(steerMotor, Set(90)).Times(4);
    swerve->calculate(0, 1, 0);
    swerve->update(0.01);

    EXPECT_CALL(driveMotor, Set(-1)).Times(4);
    EXPECT_CALL(steerMotor, Set(45)).Times(4);
    swerve->calculate(-1, -1, 0);
    swerve->update(0.01);
}
