#include "CORESwerveDrive.h"
#include "COREControl/COREPID.h"
#include "COREUtilities/COREMath.h"
#include "WPILib.h"
#include "ctre/Phoenix.h"

using namespace CORE;

CORESwerve::CORESwerve(double trackWidth, double wheelBase,
                       SwerveModule *leftFrontModule,
                       SwerveModule *leftBackModule,
                       SwerveModule *rightBackModule,
                       SwerveModule *rightFrontModule) :
        m_leftFrontModule(leftFrontModule),
        m_leftBackModule(leftBackModule),
        m_rightBackModule(rightBackModule),
        m_rightFrontModule(rightFrontModule) {
    m_trackwidth = trackWidth;
    m_wheelbase = wheelBase;
}

void CORESwerve::calculate(double forward, double strafeRight, double rotateClockwise) {
    if(forward == 0 && strafeRight == 0 && rotateClockwise == 0) {
        rightFrontModuleSpeed = 0;
        leftFrontModuleSpeed = 0;
        leftBackModuleSpeed = 0;
        rightBackModuleSpeed = 0;
        return;
    }
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
    If it does, set the angle to be the angle plus another 180 degrees, then take the remainder to
    make sure that the wheels don't make multiple rotations. Sets speed to negative*/

    double MAX_WHEEL_INVERT_SPEED = 1;

    if (abs(rightFrontModuleSpeed) <= MAX_WHEEL_INVERT_SPEED) { 
        if (fabs(rightFrontModuleAngle - m_rightFrontModule->getAngle()) > 90 &&
                fabs(rightFrontModuleAngle - m_rightFrontModule->getAngle()) < 270) {
            rightFrontModuleAngle = fmod((rightFrontModuleAngle + 180), 360);
            rightFrontModuleSpeed = -rightFrontModuleSpeed;
        }
    }

    if (abs(leftFrontModuleSpeed) <= MAX_WHEEL_INVERT_SPEED) { 
        if (fabs(leftFrontModuleAngle - m_leftFrontModule->getAngle()) > 90 &&
                fabs(leftFrontModuleAngle - m_leftFrontModule->getAngle()) < 270) {
            leftFrontModuleAngle = fmod((leftFrontModuleAngle + 180), 360);
            leftFrontModuleSpeed = -leftFrontModuleSpeed;
        }
    }

    if (abs(leftBackModuleSpeed) <= MAX_WHEEL_INVERT_SPEED){
        if (fabs(leftBackModuleAngle - m_leftBackModule->getAngle()) > 90 &&
                fabs(leftBackModuleAngle - m_leftBackModule->getAngle()) < 270) {
            leftBackModuleAngle = fmod((leftBackModuleAngle + 180), 360);
            leftBackModuleSpeed = -leftBackModuleSpeed;
        }
    }

    if (abs(rightBackModuleSpeed) <= MAX_WHEEL_INVERT_SPEED) {
        if (fabs(rightBackModuleAngle - m_rightBackModule->getAngle()) > 90 &&
                fabs(rightBackModuleAngle - m_rightBackModule->getAngle()) < 270) {
            rightBackModuleAngle = fmod((rightBackModuleAngle + 180), 360);
            rightBackModuleSpeed = -rightBackModuleSpeed;
        }
    }
}

CORESwerve::SwerveModule::SwerveModule(TalonSRX *driveMotor, TalonSRX *steerMotor, double angleOffset) :
        m_speedPIDController(0, 0, 0),
        m_anglePIDController(0, 0, 0),
        m_driveMotor(driveMotor),
        m_steerMotor(steerMotor),
        m_angleOffset(angleOffset) {
}


double CORESwerve::SwerveModule::getAngle(bool raw) {
    //Multiplying by 360 degrees and dividing by five volts
	//GetSensorCollection replaced getAnalogInRaw
    double angle = m_steerMotor->GetSensorCollection().GetAnalogInRaw() * (360.0 / 1025.0);
    if(raw) {
        return angle;
    } else {
        return angle - m_angleOffset;
    }

}

void CORESwerve::SwerveModule::setAnglePID(double p, double i, double d) {
    m_anglePIDController.setProportionalConstant(p);
    m_anglePIDController.setIntegralConstant(i);
    m_anglePIDController.setDerivativeConstant(d);
}

void CORESwerve::tank(double speed, double rot){
//    tank(COREEtherDrive::calculate(speed, rot, .1));
}


void CORESwerve::SwerveModule::drive(double magnitude, double direction, double dt) {
    double steerMotorOutput;
    if(dt == -1) {
        steerMotorOutput = m_anglePIDController.calculate(COREVector::FromCompassDegrees(getAngle()),
                                         COREVector::FromCompassDegrees(direction));
    } else {
        steerMotorOutput = m_anglePIDController.calculate(COREVector::FromCompassDegrees(getAngle()),
                                                         COREVector::FromCompassDegrees(direction), dt);
    }
    //TODO check percent output
    m_steerMotor->Set(ControlMode::PercentOutput, steerMotorOutput);
    m_driveMotor->Set(ControlMode::PercentOutput, magnitude);

}

string CORESwerve::SwerveModule::print() {
    string text = "\n\tSteer Motor Speed: " + to_string(m_steerMotor->GetSelectedSensorVelocity(0)); //TODO Change 0 to some other value
    text += "\n\tSteer Angle Offset: " + to_string(m_angleOffset);
    text += "\n\tSteer PID:";
    text += "\n\t\tkP: " + to_string(m_anglePIDController.getProportionalConstant());
    text += " kI: " + to_string(m_anglePIDController.getIntegralConstant());
    text += " kD: " + to_string(m_anglePIDController.getDerivativeConstant());
    text += " kF: " + to_string(m_anglePIDController.getFeedForwardConstant());
    text += "\n\t\tMistake: " + to_string(m_anglePIDController.getMistake());
    return text;
}

void CORESwerve::SwerveModule::setAngleOffset(double angleOffset) {
    m_angleOffset = angleOffset;
}

void CORESwerve::SwerveModule::zeroAngle() {
    m_angleOffset = -getAngle(true);
}

void CORESwerve::update(double dt){
    m_leftFrontModule->drive(leftFrontModuleSpeed, leftFrontModuleAngle, dt);
    m_rightFrontModule->drive(rightFrontModuleSpeed, rightFrontModuleAngle, dt);
    m_rightBackModule->drive(rightBackModuleSpeed, rightBackModuleAngle, dt);
    m_leftBackModule->drive(leftBackModuleSpeed, leftBackModuleAngle, dt);
}

string CORESwerve::print() {
    string text = "Swerve Drive Status:";
    text += "\nFront Right Module\n\tRequested Angle: " + to_string(rightFrontModuleAngle) + "\tActual: "
            + to_string(m_rightFrontModule->getAngle()) + "\n\tRequested Speed: " + to_string(rightFrontModuleSpeed);
    text += m_rightFrontModule->print();
    text += "\nFront Left Module\n\tRequested Angle: " + to_string(leftFrontModuleAngle) + "\tActual: "
            + to_string(m_leftFrontModule->getAngle()) + "\n\tRequested Speed: " + to_string(leftFrontModuleSpeed);
    text += m_leftFrontModule->print();
    text += "\nBack Right Module\n\tRequested Angle: " + to_string(rightBackModuleAngle) + "\tActual: "
            + to_string(m_rightBackModule->getAngle()) + "\n\tRequested Speed: " + to_string(rightBackModuleSpeed);
    text += m_rightBackModule->print();
    text += "\nBack Left Module\n\tRequested Angle: " + to_string(leftBackModuleAngle) + "\tActual: "
            + to_string(m_leftBackModule->getAngle()) + "\n\tRequested Speed: " + to_string(leftBackModuleSpeed);
    text += m_leftBackModule->print();
    return text;
}

void CORESwerve::setSteerPID(double kp, double ki, double kd) {
    m_leftFrontModule->setAnglePID(kp, ki, kd);
    m_rightFrontModule->setAnglePID(kp, ki, kd);
    m_rightBackModule->setAnglePID(kp, ki, kd);
    m_leftBackModule->setAnglePID(kp, ki, kd);
}

void CORESwerve::zeroOffsets() {
    m_leftFrontModule->zeroAngle();
    m_rightFrontModule->zeroAngle();
    m_rightBackModule->zeroAngle();
    m_leftBackModule->zeroAngle();
}
