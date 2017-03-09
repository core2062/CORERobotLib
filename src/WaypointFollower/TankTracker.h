#pragma once

#include "Position2d.h"
#include "TankKinematics.h"
#include "InterpolatingMap.h"
#include "CANTalon.h"
#include "AHRS.h"
#include "../COREHardware/CORETimer.h"
#include "../CORELog.h"
#include <thread>

using namespace CORE;

class TankTracker{
protected:
	InterpolatingTreeMap m_data;
	Position2d::Delta m_velocity = {0,0,0};

private:
    const double m_targetLoopTime = 0.01;
    mutex m_loopLock;
	mutex m_timerLock;
	double m_leftPrev = 0;
	double m_rightPrev = 0;
    CORETimer m_loopTimer;
	CANTalon * m_left;
	CANTalon * m_right;
	AHRS * m_gyro;
	thread * m_mainLoop;
	static TankTracker * m_instance;
	TankTracker();

public:
	static TankTracker* GetInstance();
	void init(CANTalon * left, CANTalon * right, AHRS * gyro);
	~TankTracker(){
		m_left = nullptr;
		m_right = nullptr;
		m_gyro = nullptr;
	}

	void reset(double time, Position2d initial);

	void start();
    void stop();

	void loop();

	Position2d getFieldToVehicle(double time);

	Position2d getLatestFieldToVehicle();

	void addData(double time, Position2d data, Position2d::Delta vel);

	Position2d generateOdometry(double leftDelta, double rightDelta, Rotation2d heading);

	std::pair<double, double> getEncoderInches();
	std::pair<double, double> getEncoderSpeed();

	Rotation2d getGyroAngle();
};
