#include "CORESwerveDrive.h"
#include "COREControl/COREPID.h"
#include "COREUtilities/COREMath.h"
#include "WPILib.h"
#include "ctre/Phoenix.h"
#include <cmath>

using namespace CORE;

CORESwerve::CORESwerve(double trackWidth, double wheelBase, double wheelDiameter,
					   double ticksToRotation,
                       SwerveModule *leftFrontModule,
                       SwerveModule *leftBackModule,
                       SwerveModule *rightBackModule,
                       SwerveModule *rightFrontModule) :
        m_frontLeftModule(leftFrontModule),
        m_backLeftModule(leftBackModule),
        m_backRightModule(rightBackModule),
        m_frontRightModule(rightFrontModule),
        m_leftFrontModuleOffset("Front Left Module Offset"),
        m_leftBackModuleOffset("Back Left Module Offset"),
        m_rightBackModuleOffset("Back Right Module Offset"),
        m_rightFrontModuleOffset("Front Right Module Offset") {
    if(!(m_frontLeftModule && m_backLeftModule && m_backRightModule && m_frontRightModule)) {
        CORELog::logError("A module passed to CORESwerve is a nullptr!");
    }
    m_trackwidth = trackWidth;
    m_wheelbase = wheelBase;
    m_wheelCircumference = wheelDiameter * PI;
    m_ticksToRotation = ticksToRotation;

    m_frontLeftModule->setAngleOffset(m_leftFrontModuleOffset.Get());
    m_frontRightModule->setAngleOffset(m_rightFrontModuleOffset.Get());
    m_backLeftModule->setAngleOffset(m_leftBackModuleOffset.Get());
    m_backRightModule->setAngleOffset(m_rightBackModuleOffset.Get());
}

CORESwerve::SwerveModule::SwerveModule(TalonSRX *driveMotor, TalonSRX *steerMotor) :
        m_speedPIDController(0, 0, 0),
        m_anglePIDController(0, 0, 0),
        m_driveMotor(driveMotor),
        m_steerMotor(steerMotor) {
}

double CORESwerve::SwerveModule::getAngle(bool raw) {
	//Multiplying by 360 degrees and dividing by five volts
	//GetSensorCollection replaced getAnalogInRaw
	double angle = m_steerMotor->GetSensorCollection().GetAnalogInRaw()
			* (360.0 / 1025.0);
	if (raw) {
		return angle;

	} else {
		return angle - m_angleOffset;
	}

}

COREVector CORESwerve::SwerveModule::inverseKinematics(
		double wheelCircumference, double ticksToRotation) {
	//Calculates the individual vector of each of the modules
	//Theta may be change in angle or the total angle
	double magnitude = (getTotalTicks() * wheelCircumference) / ticksToRotation
			- m_lastMagnitude;
	double angle = getAngle() - m_lastAngle;
	m_lastAngle += angle;
	m_lastMagnitude += magnitude;
	return COREVector(magnitude, angle);
}

void CORESwerve::SwerveModule::zeroAngle() {
	m_angleOffset = -getAngle(true);
}

double CORESwerve::SwerveModule::getTotalTicks() {
	return m_steerMotor->GetSelectedSensorPosition(0);
}

void CORESwerve::SwerveModule::setAnglePID(double p, double i, double d) {
	m_anglePIDController.setProportionalConstant(p);
	m_anglePIDController.setIntegralConstant(i);
	m_anglePIDController.setDerivativeConstant(d);
}

void CORESwerve::SwerveModule::drive(COREVector vector, double dt) {
    double steerMotorOutput;
    if(dt == -1) {
        steerMotorOutput = m_anglePIDController.calculate(COREVector::FromCompassDegrees(getAngle()), vector);
    } else {
        steerMotorOutput = m_anglePIDController.calculate(COREVector::FromCompassDegrees(getAngle()), vector, dt);
    }
    //TODO check percent output
    m_steerMotor->Set(ControlMode::PercentOutput, steerMotorOutput);
    m_driveMotor->Set(ControlMode::PercentOutput, vector.GetMagnitude());
}

string CORESwerve::SwerveModule::print() {
	string text = "\n\tSteer Motor Speed: "
			+ to_string(m_steerMotor->GetSelectedSensorVelocity(0)); //TODO Change 0 to some other value
	text += "\n\tSteer Angle Offset: " + to_string(m_angleOffset);
	text += "\n\tSteer PID:";
	text += "\n\t\tkP: "
			+ to_string(m_anglePIDController.getProportionalConstant());
	text += " kI: " + to_string(m_anglePIDController.getIntegralConstant());
	text += " kD: " + to_string(m_anglePIDController.getDerivativeConstant());
	text += " kF: " + to_string(m_anglePIDController.getFeedForwardConstant());
	text += "\n\t\tMistake: " + to_string(m_anglePIDController.getMistake());
	return text;
}

void CORESwerve::SwerveModule::setAngleOffset(double angleOffset) {
	m_angleOffset = angleOffset;
}


void CORESwerve::calculate(double x, double y, double theta) {
    COREVector m_frontRight, m_frontLeft, m_backRight, m_backLeft;
    if(y == 0 && x == 0 && theta == 0) {
        m_frontRight.SetMagnitude(0);
        m_frontLeft.SetMagnitude(0);
        m_backRight.SetMagnitude(0);
        m_backLeft.SetMagnitude(0);
        return;
    }
    double r = sqrt(pow(m_wheelbase, 2) + pow(m_trackwidth, 2));
    double a = x - theta * (m_wheelbase / r);
    double b = x + theta * (m_wheelbase / r);
    double c = y - theta * (m_trackwidth / r);
    double d = y + theta * (m_trackwidth / r);

    double frontRightModuleSpeed = sqrt(pow(b, 2) + pow(c, 2));
    double frontLeftModuleSpeed = sqrt(pow(b, 2) + pow(d, 2));
    double backLeftModuleSpeed = sqrt(pow(a, 2) + pow(d, 2));
    double backRightModuleSpeed = sqrt(pow(a, 2) + pow(c, 2));

    double frontRightModuleAngle = arctan(b, c);
    double frontLeftModuleAngle = arctan(b, d);
    double backLeftModuleAngle = arctan(a, d);
    double backRightModuleAngle = arctan(a, c);

    double maxSpeed = frontRightModuleSpeed;

    if (frontLeftModuleSpeed > maxSpeed) {
        maxSpeed = frontLeftModuleSpeed;
    }
    if (backLeftModuleSpeed > maxSpeed) {
        maxSpeed = backLeftModuleSpeed;
    }
    if (backRightModuleSpeed > maxSpeed) {
        maxSpeed = backRightModuleSpeed;
    }
    if (maxSpeed > 1 && maxSpeed != 0) {
        frontRightModuleSpeed /= maxSpeed;
        frontLeftModuleSpeed /= maxSpeed;
        backLeftModuleSpeed /= maxSpeed;
        backRightModuleSpeed /= maxSpeed;
    }
    
    m_frontRight = COREVector::FromCompassDegrees(frontRightModuleAngle, frontRightModuleSpeed);
    m_frontLeft = COREVector::FromCompassDegrees(frontLeftModuleAngle, frontLeftModuleSpeed);
    m_backRight = COREVector::FromCompassDegrees(backRightModuleAngle, backRightModuleSpeed);
    m_backLeft = COREVector::FromCompassDegrees(backLeftModuleAngle, backLeftModuleSpeed);

	double MAX_WHEEL_INVERT_SPEED = 1;

    if(m_frontRight.GetMagnitude() <= MAX_WHEEL_INVERT_SPEED) {
        if(abs(COREVector::FromCompassDegrees(m_frontRightModule->getAngle())
                       .ShortestRotationTo(m_frontRight).GetDegrees()) > 45) {
            m_frontRight.MagnitudeInverse();
            m_frontRight.RotationInverse();
        }
    }

    if(m_frontLeft.GetMagnitude() <= MAX_WHEEL_INVERT_SPEED) {
        if(abs(COREVector::FromCompassDegrees(m_frontLeftModule->getAngle())
                       .ShortestRotationTo(m_frontLeft).GetDegrees()) > 45) {
            m_frontLeft.MagnitudeInverse();
            m_frontLeft.RotationInverse();
        }
    }

    if(m_backRight.GetMagnitude() <= MAX_WHEEL_INVERT_SPEED) {
        if(abs(COREVector::FromCompassDegrees(m_backRightModule->getAngle())
                       .ShortestRotationTo(m_backRight).GetDegrees()) > 45) {
            m_backRight.MagnitudeInverse();
            m_backRight.RotationInverse();
        }
    }

    if(m_backLeft.GetMagnitude() <= MAX_WHEEL_INVERT_SPEED) {
        if(abs(COREVector::FromCompassDegrees(m_backLeftModule->getAngle())
                       .ShortestRotationTo(m_backLeft).GetDegrees()) > 45) {
            m_backLeft.MagnitudeInverse();
            m_backLeft.RotationInverse();
        }
    }
    
}

COREVector CORESwerve::inverseKinematics() {
	//Adds the up all of the vector of each of the modules to get a total vector
	double r = sqrt(pow(m_wheelbase, 2) + pow(m_trackwidth, 2));

	COREVector leftFront = m_frontLeftModule->inverseKinematics(m_wheelCircumference, m_ticksToRotation);
	leftFront.SetX(leftFront.GetX() - m_frontLeftModule->getAngle() * (m_wheelbase / r));
	leftFront.SetY(leftFront.GetY() - m_frontLeftModule->getAngle() * (m_trackwidth / r));
	SmartDashboard::PutNumber("Left Front X: ", leftFront.GetX());
	SmartDashboard::PutNumber("Left Front Y: ", leftFront.GetY());

	COREVector rightFront = m_frontRightModule->inverseKinematics(m_wheelCircumference, m_ticksToRotation);
	rightFront.SetX(rightFront.GetX() - m_frontRightModule->getAngle() * (m_wheelbase / r));
	rightFront.SetY(rightFront.GetY() - m_frontRightModule->getAngle() * (m_trackwidth / r));
	SmartDashboard::PutNumber("Right Front X: ", rightFront.GetX());
	SmartDashboard::PutNumber("Right Front Y: ", rightFront.GetY());

	COREVector leftBack = m_backLeftModule->inverseKinematics(m_wheelCircumference, m_ticksToRotation);
	leftBack.SetX(leftBack.GetX() - m_backLeftModule->getAngle() * (m_wheelbase / r));
	leftBack.SetY(leftBack.GetY() - m_backLeftModule->getAngle() * (m_trackwidth / r));
	SmartDashboard::PutNumber("Left Back X: ", leftBack.GetX());
	SmartDashboard::PutNumber("Left Back Y: ", leftBack.GetY());

	COREVector rightBack = m_backRightModule->inverseKinematics(m_wheelCircumference, m_ticksToRotation);
	rightBack.SetX(rightBack.GetX() - m_backRightModule->getAngle() * (m_wheelbase / r));
	rightBack.SetY(rightBack.GetY() - m_backRightModule->getAngle() * (m_trackwidth / r));
	SmartDashboard::PutNumber("Right Back X: ", rightBack.GetX());
	SmartDashboard::PutNumber("Right Back Y: ", rightBack.GetY());

	return leftFront.AddVector(rightFront.AddVector(leftBack.AddVector(rightBack)));

}

void CORESwerve::update(double dt) {
	m_frontLeftModule->drive(m_frontLeft, dt);
	m_frontRightModule->drive(m_frontRight, dt);
	m_backRightModule->drive(m_backRight, dt);
	m_backLeftModule->drive(m_backLeft, dt);
}

void CORESwerve::setSteerPID(double kp, double ki, double kd) {
	m_frontLeftModule->setAnglePID(kp, ki, kd);
	m_frontRightModule->setAnglePID(kp, ki, kd);
	m_backRightModule->setAnglePID(kp, ki, kd);
	m_backLeftModule->setAnglePID(kp, ki, kd);
}

void CORESwerve::zeroOffsets() {
    m_leftFrontModuleOffset.Set(m_frontLeftModule->getAngle(true));
    m_frontLeftModule->setAngleOffset(m_leftFrontModuleOffset.Get());
    
    m_rightFrontModuleOffset.Set(m_frontRightModule->getAngle(true));
    m_frontRightModule->setAngleOffset(m_rightFrontModuleOffset.Get());

    m_leftBackModuleOffset.Set(m_backLeftModule->getAngle(true));
    m_backLeftModule->setAngleOffset(m_leftBackModuleOffset.Get());

    m_rightBackModuleOffset.Set(m_backRightModule->getAngle(true));
    m_backRightModule->setAngleOffset(m_rightBackModuleOffset.Get());
}
