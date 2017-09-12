#include "CORESwerveDrive.h"
#include "COREUtilities/COREMath.h"
#include "WPILib.h"

using namespace CORE;

CORESwerve::CORESwerve(double trackWidth, double wheelBase,
                       CORESwerve::SwerveModule &leftFrontModule,
                       CORESwerve::SwerveModule &leftBackModule,
                       CORESwerve::SwerveModule &rightBackModule,
                       CORESwerve::SwerveModule &rightFrontModule) :
        m_leftFrontModule(&leftFrontModule),
        m_leftBackModule(&leftBackModule),
        m_rightBackModule(&rightBackModule),
        m_rightFrontModule(&rightFrontModule) {
    m_modules.push_back(m_leftFrontModule);
    m_modules.push_back(m_leftBackModule);
    m_modules.push_back(m_rightBackModule);
    m_modules.push_back(m_rightFrontModule);
}

void CORESwerve::calculate(double forward, double strafeRight, double rotateClockwise) {
    double r = sqrt(pow(m_wheelbase, 2) + pow(m_trackwidth, 2));
    double a = strafeRight - rotateClockwise * (m_wheelbase / r);
    double b = strafeRight + rotateClockwise * (m_wheelbase / r);
    double c = forward - rotateClockwise * (m_trackwidth / r);
    double d = forward + rotateClockwise * (m_trackwidth / r);

    rightFrontModuleSpeed = sqrt(pow(b, 2) + pow(c, 2));
    leftFrontModuleSpeed = sqrt(pow(b, 2) + pow(d, 2));
    leftBackModuleSpeed = sqrt(pow(a, 2) + pow(d, 2));
    rightBackModuleSpeed = sqrt(pow(a, 2) + pow(c, 2));

    rightFrontModuleAngle = arctan(b, c);
    leftFrontModuleAngle = arctan(b, d);
    leftBackModuleAngle = arctan(a, d);
    rightBackModuleAngle = arctan(a, c);

    double maxSpeed = rightFrontModuleSpeed;

    if (leftFrontModuleSpeed > maxSpeed) {
        maxSpeed = leftFrontModuleSpeed;
    }
    if (leftBackModuleSpeed > maxSpeed) {
        maxSpeed = leftBackModuleSpeed;
    }
    if (rightBackModuleSpeed > maxSpeed) {
        maxSpeed = rightBackModuleSpeed;
    }
    if (maxSpeed > 1 && maxSpeed != 0) {
        rightFrontModuleSpeed /= maxSpeed;
        leftFrontModuleSpeed /= maxSpeed;
        leftBackModuleSpeed /= maxSpeed;
        rightBackModuleSpeed /= maxSpeed;
    }

    /*Code to determine if the motors should be set to be in reverse and rotate the wheels accordingly
    First checks to see if the angle from the driver would require turning more than 90 degrees
    If it does, set the angle to be the angle plus another 180 degrees, then take the remainder to make sure that the wheels don't make
    multiple rotations
    Sets speed to negative (I guess this would make it velocity)*/
    /*
    if (fabs(rightFrontModuleAngle - m_rightFrontModule->getAngle()) > 90 &&
            fabs(rightFrontModuleAngle - m_rightFrontModule->getAngle()) < 270) {
        rightFrontModuleAngle = fmod((rightFrontModuleAngle + 180), 360);
        rightFrontModuleSpeed = -rightFrontModuleSpeed;
    }

    if (fabs(leftFrontModuleAngle - m_leftFrontModule->getAngle()) > 90 &&
            fabs(leftFrontModuleAngle - m_leftFrontModule->getAngle()) < 270) {
        leftFrontModuleAngle = fmod((leftFrontModuleAngle + 180), 360);
        leftFrontModuleSpeed = -leftFrontModuleSpeed;
    }

    if (fabs(leftBackModuleAngle - m_leftBackModule->getAngle()) > 90 &&
            fabs(leftBackModuleAngle - m_leftBackModule->getAngle()) < 270) {
        leftBackModuleAngle = fmod((leftBackModuleAngle + 180), 360);
        leftBackModuleSpeed = -leftBackModuleSpeed;
    }

    if (fabs(rightBackModuleAngle - m_rightBackModule->getAngle()) > 90 &&
            fabs(rightBackModuleAngle - m_rightBackModule->getAngle()) < 270) {
        rightBackModuleAngle = fmod((rightBackModuleAngle + 180), 360);
        rightBackModuleSpeed = -rightBackModuleSpeed;
    }
    */
}

double CORESwerve::SwerveModule::getAngle() {
    //Multiplying by 360 degrees and dividing by five volts
    return m_steerMotor->GetAnalogInRaw() * (360.0 / 1025.0);
}

void CORESwerve::SwerveModule::setAnglePID(double p, double i, double d) {
    m_anglePIDController.setProportionalConstant(p);
    m_anglePIDController.setIntegralConstant(i);
    m_anglePIDController.setDerivativeConstant(d);
}

void CORESwerve::tank(double speed, double rot){
    tank(COREEtherDrive::calculate(speed, rot, .1));
}

void CORESwerve::tank(VelocityPair speeds){
    leftFrontModuleAngle = 0;
    rightFrontModuleAngle = 0;
    leftBackModuleAngle = 0;
    rightBackModuleAngle = 0;
    leftFrontModuleSpeed = speeds.left;
    leftBackModuleSpeed = speeds.left;
    rightFrontModuleSpeed = speeds.right;
    rightBackModuleSpeed = speeds.right;
}

void CORESwerve::SwerveModule::drive(double magnitude, double direction) {
    m_steerMotor->Set(m_anglePIDController.calculate(Rotation2d::fromCompassDegrees(getAngle()),
                                                     Rotation2d::fromCompassDegrees(direction)));
    m_driveMotor->Set(magnitude);

}

void CORESwerve::update(){
    SmartDashboard::PutNumber("Left Front Module P value", m_leftFrontModule->m_anglePIDController.getProportional());
    m_leftFrontModule->drive(leftFrontModuleSpeed, leftFrontModuleAngle);
    SmartDashboard::PutNumber("Left Front Module Speed", leftFrontModuleSpeed);
    SmartDashboard::PutNumber("Left Front Module Angle", leftFrontModuleAngle);
    m_rightFrontModule->drive(rightFrontModuleSpeed, rightFrontModuleAngle);
    SmartDashboard::PutNumber("Right Front Module Speed", rightFrontModuleSpeed);
    SmartDashboard::PutNumber("Right Front Module Angle", rightFrontModuleAngle);
    m_rightBackModule->drive(rightBackModuleSpeed, rightBackModuleAngle);
    m_leftBackModule->drive(leftBackModuleSpeed, leftBackModuleAngle);
}