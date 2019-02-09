#pragma once

#include "Position2d.h"
#include "TankKinematics.h"
#include "InterpolatingMap.h"
#include <ctre/phoenix.h>
#include "AHRS.h"
#include "../src/COREUtilities/CORETimer.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/CORETask.h"
#include <frc/WPILib.h>
#include <thread>
#include "CORELogging/COREDataLog.h"

using namespace CORE;

class TankTracker : public CORETask {
protected:
	InterpolatingTreeMap m_data;
	Position2d::Delta m_velocity = {0,0,0};

private:
    const double m_targetLoopHz = 200; //If this is changed, be sure to adjust NavX constructor accordingly
    double m_targetLoopTime = 0.01;
    atomic<bool> m_loopEnabled;
    std::mutex m_loopLock;
    std::mutex m_dataLock;
	std::mutex m_timerLock;
	double m_leftPrev = 0;
	double m_rightPrev = 0;
    CORETimer m_loopTimer;
	TalonSRX * m_left = nullptr;
	TalonSRX * m_right = nullptr;
	AHRS * m_gyro = nullptr;
	thread * m_mainLoop = nullptr;
	static TankTracker * m_instance;
	TankTracker();
	void Start();
    void Stop();
	void AddData(double time, Position2d data, Position2d::Delta vel);
	COREDataLogger log;

public:
	bool doLog = false;
	static TankTracker* GetInstance();
	void Init(TalonSRX * left, TalonSRX * right, AHRS * gyro);
	~TankTracker();
	void Reset(double time, Position2d initial);
	void Loop();
	Position2d GetFieldToVehicle(double time);
	Position2d GetLatestFieldToVehicle();
	Position2d GenerateOdometry(double leftDelta, double rightDelta, Rotation2d heading);
	std::pair<double, double> GetEncoderInches();
	std::pair<double, double> GetEncoderSpeed();
	Rotation2d GetGyroAngle();
	void AutonInitTask() override;
	void AutonEndTask() override;
	void TeleopInitTask() override;
	void PostLoopTask() override;
	void TeleopEndTask() override;
};