#include "CORESwerveDrive.h"
#include "COREControl/COREPID.h"
#include "COREUtilities/COREMath.h"
#include "WPILib.h"
#include "ctre/Phoenix.h"

using namespace CORE;

CORESwerve::CORESwerve(double trackWidth, double wheelBase,
		double wheelDiameter, double ticksToRotation,
		SwerveModule *leftFrontModule, SwerveModule *leftBackModule,
		SwerveModule *rightBackModule, SwerveModule *rightFrontModule) :
		m_leftFrontModule(leftFrontModule), m_leftBackModule(leftBackModule), m_rightBackModule(
				rightBackModule), m_rightFrontModule(rightFrontModule) {
	if (!(m_leftFrontModule && m_leftBackModule && m_rightBackModule
			&& m_rightFrontModule)) {
		CORELog::logError("A module passed to CORESwerve is a nullptr!");
	}
	m_trackwidth = trackWidth;
	m_wheelbase = wheelBase;
	m_wheelCircumference = wheelDiameter * PI;
	m_ticksToRotation = ticksToRotation;
	m_wheelDiameter = wheelDiameter;

}

SwerveVectorPair::SwerveVectorPair(double ang, double pos) {
	angle = ang;
	position = pos;
}

CORESwerve::SwerveModule::SwerveModule(TalonSRX *driveMotor,
		TalonSRX *steerMotor, double angleOffset) :
		m_speedPIDController(0, 0, 0), m_anglePIDController(0, 0, 0), m_driveMotor(
				driveMotor), m_steerMotor(steerMotor), m_angleOffset(
				angleOffset) {
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

double CORESwerve::SwerveModule::getPosition(double position) {
	return position;
}

Position2d::Delta CORESwerve::SwerveModule::forwardKinematics() {
	double deltaPosition =
			m_driveMotor->GetSensorCollection().GetPulseWidthPosition()
					- getPosition(
							m_driveMotor->GetSensorCollection().GetPulseWidthPosition());
	double deltaRotation = m_steerMotor->GetSensorCollection().GetAnalogInRaw()
			- getAngle();

	return Position2d::Delta(deltaPosition, 0, deltaRotation);
}

Position2d CORESwerve::SwerveModule::integrateForwardKinematics(Position2d pos,
		double deltaPosition, COREVector heading) {
	Position2d::Delta withGyro = forwardKinematics(deltaPosition,
			pos.getRotation().RotationInverse().RotateBy(heading).GetRadians());
	return pos.transformBy(Position2d::fromVelocity(withGyro));
}

SwerveVectorPair CORESwerve::SwerveModule::inverseKinematics(double deltaAngle,
		double deltaPosition) {
	return SwerveVectorPair(deltaAngle, deltaPosition);

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

void CORESwerve::SwerveModule::drive(double magnitude, double direction,
		double dt) {
	double steerMotorOutput;
	if (dt == -1) {
		steerMotorOutput = m_anglePIDController.calculate(
				COREVector::FromCompassDegrees(getAngle()),
				COREVector::FromCompassDegrees(direction));
	} else {
		steerMotorOutput = m_anglePIDController.calculate(
				COREVector::FromCompassDegrees(getAngle()),
				COREVector::FromCompassDegrees(direction), dt);
	}
	m_steerMotor->Set(ControlMode::PercentOutput, steerMotorOutput);
	m_driveMotor->Set(ControlMode::PercentOutput, magnitude);

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

void CORESwerve::calculate(double forward, double strafeRight,
		double rotateClockwise) {
	if (forward == 0 && strafeRight == 0 && rotateClockwise == 0) {
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
		if (fabs(rightFrontModuleAngle - m_rightFrontModule->getAngle()) > 90
				&& fabs(rightFrontModuleAngle - m_rightFrontModule->getAngle())
						< 270) {
			rightFrontModuleAngle = fmod((rightFrontModuleAngle + 180), 360);
			rightFrontModuleSpeed = -rightFrontModuleSpeed;
		}
	}

	if (abs(leftFrontModuleSpeed) <= MAX_WHEEL_INVERT_SPEED) {
		if (fabs(leftFrontModuleAngle - m_leftFrontModule->getAngle()) > 90
				&& fabs(leftFrontModuleAngle - m_leftFrontModule->getAngle())
						< 270) {
			leftFrontModuleAngle = fmod((leftFrontModuleAngle + 180), 360);
			leftFrontModuleSpeed = -leftFrontModuleSpeed;
		}
	}

	if (abs(leftBackModuleSpeed) <= MAX_WHEEL_INVERT_SPEED) {
		if (fabs(leftBackModuleAngle - m_leftBackModule->getAngle()) > 90
				&& fabs(leftBackModuleAngle - m_leftBackModule->getAngle())
						< 270) {
			leftBackModuleAngle = fmod((leftBackModuleAngle + 180), 360);
			leftBackModuleSpeed = -leftBackModuleSpeed;
		}
	}

	if (abs(rightBackModuleSpeed) <= MAX_WHEEL_INVERT_SPEED) {
		if (fabs(rightBackModuleAngle - m_rightBackModule->getAngle()) > 90
				&& fabs(rightBackModuleAngle - m_rightBackModule->getAngle())
						< 270) {
			rightBackModuleAngle = fmod((rightBackModuleAngle + 180), 360);
			rightBackModuleSpeed = -rightBackModuleSpeed;
		}
	}
}

void CORESwerve::calculateInverseKinematics(double fudgeFactor) {
	//TODO put these in the modules instead of in the swerve drive class
	double r = sqrt(pow(m_trackwidth, 2) + pow(m_wheelbase, 2));

	//Sets the change in x for each of the modules
	leftFrontDeltaX = ((cos(leftFrontModuleAngle) * fudgeFactor
			* fmod(m_wheelCircumference * m_leftFrontModule->getTotalTicks(),
					360) / m_ticksToRotation)
			+ leftFrontModuleAngle * m_wheelbase / r);
	rightFrontDeltaX = ((cos(rightFrontModuleAngle) * fudgeFactor
			* fmod(m_wheelCircumference * m_rightFrontModule->getTotalTicks(),
					360) / m_ticksToRotation)
			+ rightFrontModuleAngle * m_wheelbase / r);
	leftBackDeltaX = ((cos(leftBackModuleAngle) * fudgeFactor
			* fmod(m_wheelCircumference * m_leftBackModule->getTotalTicks(),
					360) / m_ticksToRotation)
			- leftBackModuleAngle * m_wheelbase / r);
	rightBackDeltaX = ((cos(rightBackModuleAngle) * fudgeFactor
			* fmod(m_wheelCircumference * m_rightBackModule->getTotalTicks(),
					360) / m_ticksToRotation)
			- rightBackModuleAngle * m_wheelbase / r);

	SmartDashboard::PutNumber("Left Front Delta X", leftFrontDeltaX);
	SmartDashboard::PutNumber("Right Front Delta X", rightFrontDeltaX);
	SmartDashboard::PutNumber("Left Back Delta X", leftBackDeltaX);
	SmartDashboard::PutNumber("Right Back Delta X", rightBackDeltaX);

	//Sets the change in y for all of the modules
	leftFrontDeltaY = ((sin(leftFrontModuleAngle) * fudgeFactor
			* fmod(m_wheelCircumference * m_leftFrontModule->getTotalTicks(),
					360) / m_ticksToRotation)
			+ leftFrontModuleAngle * m_wheelbase / r);
	rightFrontDeltaY = ((sin(rightFrontModuleAngle) * fudgeFactor
			* fmod(m_wheelCircumference * m_rightFrontModule->getTotalTicks(),
					360) / m_ticksToRotation)
			- rightFrontModuleAngle * m_wheelbase / r);
	leftBackDeltaY = ((sin(leftBackModuleAngle) * fudgeFactor
			* fmod(m_wheelCircumference * m_leftBackModule->getTotalTicks(),
					360) / m_ticksToRotation)
			+ leftBackModuleAngle * m_wheelbase / r);
	rightBackDeltaY = ((sin(rightBackModuleAngle) * fudgeFactor
			* fmod(m_wheelCircumference * m_rightBackModule->getTotalTicks(),
					360) / m_ticksToRotation)
			- rightBackModuleAngle * m_wheelbase / r);

	SmartDashboard::PutNumber("Left Front Delta Y", leftFrontDeltaY);
	SmartDashboard::PutNumber("Right Front Delta Y", rightFrontDeltaY);
	SmartDashboard::PutNumber("Left Back Delta Y", leftBackDeltaY);
	SmartDashboard::PutNumber("Right Back Delta Y", rightBackDeltaY);

	rightFrontDeltaPosition = sqrt(
			pow(rightFrontDeltaY, 2) + pow(rightFrontDeltaX, 2));
	leftFrontDeltaPosition = sqrt(
			pow(leftFrontDeltaY, 2) + pow(leftFrontDeltaX, 2));
	rightBackDeltaPosition = sqrt(
			pow(rightBackDeltaY, 2) + pow(rightBackDeltaX, 2));
	leftFrontDeltaPosition = sqrt(
			pow(leftBackDeltaY, 2) + pow(leftBackDeltaX, 2));

	SmartDashboard::PutNumber("Left Front Delta Position",
			leftFrontDeltaPosition);
	SmartDashboard::PutNumber("Right Front Delta Y", rightFrontDeltaY);
	SmartDashboard::PutNumber("Left Back Delta Y", leftBackDeltaY);
	SmartDashboard::PutNumber("Right Back Delta Y", rightBackDeltaY);

	//Sets the change in rotation for all of the modules
	leftFrontDeltaRot = ((leftFrontDeltaX - leftFrontDeltaY) * fudgeFactor)
			/ m_wheelDiameter;
	rightFrontDeltaRot = ((rightFrontDeltaX - rightFrontDeltaY) * fudgeFactor)
			/ m_wheelDiameter;
	leftBackDeltaRot = ((leftBackDeltaX - leftBackDeltaY) * fudgeFactor)
			/ m_wheelDiameter;
	rightBackDeltaRot = ((rightBackDeltaX - rightBackDeltaY) * fudgeFactor)
			/ m_wheelDiameter;

	SmartDashboard::PutNumber("Left Front Delta Rot", leftFrontDeltaRot);
	SmartDashboard::PutNumber("Right Front Delta Rot", rightFrontDeltaRot);
	SmartDashboard::PutNumber("Left Back Delta Rot", leftBackDeltaRot);
	SmartDashboard::PutNumber("Right Back Delta Rot", rightBackDeltaRot);
}

Position2d::Delta CORESwerve::forwardKinematics() {
	m_leftFrontModule->forwardKinematics();
	m_rightFrontModule->forwardKinematics();
	m_leftBackModule->forwardKinematics();
	m_rightBackModule->forwardKinematics();
	return m_rightFrontModule->forwardKinematics();
}
Position2d::Delta CORESwerve::forwardKinematics(double fudgeFactor,
		double leftFrontDeltaPosition, double rightFrontDeltaPosition,
		double leftBackDeltaPosition, double rightBackDeltaPosition) {
	calculateInverseKinematics(fudgeFactor);
	m_leftFrontModule->forwardKinematics(leftFrontDeltaPosition,
			leftFrontDeltaRot);
	m_rightFrontModule->forwardKinematics(rightFrontDeltaPosition,
			rightFrontDeltaRot);
	m_leftBackModule->forwardKinematics(leftBackDeltaPosition,
			leftBackDeltaRot);
	m_rightBackModule->forwardKinematics(rightBackDeltaPosition,
			rightBackDeltaRot);
	return m_rightFrontModule->forwardKinematics(rightBackDeltaPosition,
			rightFrontDeltaRot);
}

Position2d CORESwerve::integrateForwardKinematics(Position2d pos, COREVector heading,
		double fudgeFactor) {
	calculateInverseKinematics(fudgeFactor);
	m_leftFrontModule->integrateForwardKinematics(pos, leftFrontDeltaPosition,
			heading);
	m_rightFrontModule->integrateForwardKinematics(pos, rightFrontDeltaPosition,
			heading);
	m_leftBackModule->integrateForwardKinematics(pos, leftBackDeltaPosition,
			heading);
	m_rightBackModule->integrateForwardKinematics(pos, rightBackDeltaPosition,
			heading);
	return 	m_rightFrontModule->integrateForwardKinematics(pos, rightFrontDeltaPosition,
			heading);
}

SwerveVectorPair CORESwerve::inverseKinematics(Position2d::Delta vel, double fudgeFactor) {
	calculateInverseKinematics(fudgeFactor);
	m_leftFrontModule->inverseKinematics(leftFrontDeltaRot,
			leftFrontDeltaPosition);
	m_rightFrontModule->inverseKinematics(rightFrontDeltaRot,
			rightFrontDeltaPosition);
	m_leftBackModule->inverseKinematics(leftBackDeltaRot,
			leftBackDeltaPosition);
	m_rightBackModule->inverseKinematics(rightBackDeltaRot,
			rightBackDeltaPosition);
	return 	m_rightFrontModule->inverseKinematics(rightFrontDeltaRot,
			rightFrontDeltaPosition);
}

void CORESwerve::update(double dt) {
	m_leftFrontModule->drive(leftFrontModuleSpeed, leftFrontModuleAngle, dt);
	m_rightFrontModule->drive(rightFrontModuleSpeed, rightFrontModuleAngle, dt);
	m_rightBackModule->drive(rightBackModuleSpeed, rightBackModuleAngle, dt);
	m_leftBackModule->drive(leftBackModuleSpeed, leftBackModuleAngle, dt);
}

string CORESwerve::print() {
	string text = "Swerve Drive Status:";
	text += "\nFront Right Module\n\tRequested Angle: "
			+ to_string(rightFrontModuleAngle) + "\tActual: "
			+ to_string(m_rightFrontModule->getAngle())
			+ "\n\tRequested Speed: " + to_string(rightFrontModuleSpeed);
	text += m_rightFrontModule->print();
	text += "\nFront Left Module\n\tRequested Angle: "
			+ to_string(leftFrontModuleAngle) + "\tActual: "
			+ to_string(m_leftFrontModule->getAngle()) + "\n\tRequested Speed: "
			+ to_string(leftFrontModuleSpeed);
	text += m_leftFrontModule->print();
	text += "\nBack Right Module\n\tRequested Angle: "
			+ to_string(rightBackModuleAngle) + "\tActual: "
			+ to_string(m_rightBackModule->getAngle()) + "\n\tRequested Speed: "
			+ to_string(rightBackModuleSpeed);
	text += m_rightBackModule->print();
	text += "\nBack Left Module\n\tRequested Angle: "
			+ to_string(leftBackModuleAngle) + "\tActual: "
			+ to_string(m_leftBackModule->getAngle()) + "\n\tRequested Speed: "
			+ to_string(leftBackModuleSpeed);
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
