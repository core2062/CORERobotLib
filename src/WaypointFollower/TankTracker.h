#pragma once

#include "Position2d.h"
#include "TankKinematics.h"
#include "InterpolatingMap.h"
#include "CANTalon.h"
#include "AHRS.h"

class TankTracker{
protected:
	InterpolatingTreeMap m_data;
	Position2d::Delta m_velocity = {0,0,0};

private:
	double m_leftPrev = 0;
	double m_rightPrev = 0;
	CANTalon * m_left;
	CANTalon * m_right;
	AHRS * m_gyro;


public:
	TankTracker(CANTalon * left, CANTalon * right, AHRS * gyro);

	void reset(double time, Position2d initial);

	void start();

	void loop();

	Position2d getFieldToVehicle(double time);

	Position2d getLatestFieldToVehicle();

	void addData(double time, Position2d data, Position2d::Delta vel);

	Position2d generateOdometry(double leftDelta, double rightDelta, Rotation2d heading);

	std::pair<double, double> getEncoderInches();
	std::pair<double, double> getEncoderSpeed();

	Rotation2d getGyroAngle();
};
