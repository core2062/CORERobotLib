#pragma once

#include "Position2d.h"
#include "TankKinematics.h"
#include "InterpolatingMap.h"
#include "CANTalon.h"
#include "AHRS.h"
#include "../COREHardware/CORETimer.h"
#include "../CORELog.h"
#include "../CORETask.h"
#include "WPILib.h"
#include <thread>

using namespace CORE;

class TankTracker : public CORETask {
protected:
	InterpolatingTreeMap m_data;
	Position2d::Delta m_velocity = {0,0,0};

private:
    const double m_targetLoopHz = 100; //If this is changed, be sure to adjust NavX constructor accordingly
    double m_targetLoopTime;
    atomic<bool> m_loopEnabled;
    mutex m_loopLock;
    mutex m_dataLock;
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
	void start();
    void stop();
	void addData(double time, Position2d data, Position2d::Delta vel);

public:
	static TankTracker* GetInstance();
	void init(CANTalon * left, CANTalon * right, AHRS * gyro);
	~TankTracker();
	void reset(double time, Position2d initial);
	void loop();
	Position2d getFieldToVehicle(double time);
	Position2d getLatestFieldToVehicle();
	Position2d generateOdometry(double leftDelta, double rightDelta, Rotation2d heading);
	std::pair<double, double> getEncoderInches();
	std::pair<double, double> getEncoderSpeed();
	Rotation2d getGyroAngle();
	void autonInitTask() override;
	void autonEndTask() override;
	void teleopInitTask() override;
	void postLoopTask() override;
	void teleopEndTask() override;
};
