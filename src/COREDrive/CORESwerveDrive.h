#pragma once

#include <vector>
#include "COREFramework/COREHardwareManager.h"
#include "COREDrive.h"
#include "ctre/Phoenix.h"
#include "COREHardware/CORESensor.h"
#include "COREControl/COREPID.h"
#include "COREUtilities/COREVector.h"

namespace CORE {
class CORESwerve {
public:
	class SwerveModule {
	public:
		SwerveModule(int driveMotorPortNumber, int steerMotorPortNumber, double angleOffset = 0);
		double getAngle(bool raw = false);
		void setAnglePID(double p, double i, double d);
		void setAngleOffset(double angleOffset);
		void zeroAngleOffset();
		void init();
		string print();
		void drive(double magnitude, double direction, double dt = -1);
		void resetMotors();
		void resetEncoders();
		COREPID m_speedPIDController;
		COREAnglePID m_anglePIDController;
	private:
		TalonSRX m_driveMotor;
		TalonSRX m_steerMotor;
		double m_angleOffset;
	};

	CORESwerve(double trackWidth, double wheelBase,
			int leftFrontDriveMotorPort, int leftFrontSteerMotorPort, double leftFrontAngleOffset,
			int leftBackDriveMotorPort, int leftBackSteerMotorPort, double leftBackAngleOffset,
			int rightFrontDriveMotorPort, int rightFrontSteerMotorPort, double rightFrontAngleOffset,
			int rightBackDriveMotorPort, int rightBackSteerMotorPort, double rightBackAngleOffset);
public:
	void init();
	void setSteerPID(double kp, double ki, double kd);
	void zeroAngleOffsets();
	void setAngleOffsets(double leftFrontOffset, double leftBackOffset, double rightFrontOffset, double rightBackOffset);
	void resetEncoders();
	void resetMotors();
	double getLeftFrontModuleAngle(bool raw);
	double getLeftBackModuleAngle(bool raw);
	double getRightFrontModuleAngle(bool raw);
	double getRightBackModuleAngle(bool raw);
	void calculate(double speed, double strafeRight, double rotateClockwise);
	string print();
	void setMotors(double x, double y, double theta, double gyroYaw);
	void haltMotors();
	void update(double dt = -1);
	double leftFrontModuleSpeed = 0.0;
	double rightFrontModuleSpeed = 0.0;
	double leftBackModuleSpeed = 0.0;
	double rightBackModuleSpeed = 0.0;
	double leftFrontModuleAngle = 0.0;
	double rightFrontModuleAngle = 0.0;
	double rightBackModuleAngle = 0.0;
	double leftBackModuleAngle = 0.0;

private:
	double m_wheelbase;
	double m_trackwidth;
	SwerveModule m_leftFrontModule, m_leftBackModule, m_rightBackModule, m_rightFrontModule;
};
}

