#pragma once

#include "SwervePosition2d.h"
#include "SwerveKinematics.h"
#include "SwerveInterpolatingMap.h"
#include <ctre/phoenix.h>
#include "AHRS.h"
#include "../src/COREUtilities/CORETimer.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/CORETask.h"
#include <frc/WPILib.h>
#include <thread>
#include "CORELogging/COREDataLog.h"

using namespace CORE;

class SwerveTracker : public CORETask {
protected:
	SwerveInterpolatingTreeMap m_data;
	SwervePosition2d::SwerveDelta m_velocity = {0,0,0};

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
	static SwerveTracker * m_instance;
	SwerveTracker();
	void Start();
    void Stop();
	void AddData(double time, SwervePosition2d data, SwervePosition2d::SwerveDelta vel);
	COREDataLogger log;

public:
	bool doLog = false;
	static SwerveTracker* GetInstance();
	void Init(TalonSRX * left, TalonSRX * right, AHRS * gyro);
	~SwerveTracker();
	void Reset(double time, SwervePosition2d initial);
	void Loop();
	SwervePosition2d GetFieldToVehicle(double time);
	SwervePosition2d GetLatestFieldToVehicle();
	SwervePosition2d GenerateOdometry(double leftDelta, double rightDelta, SwerveRotation2d heading);
	std::pair<double, double> GetEncoderInches();
	std::pair<double, double> GetEncoderSpeed();
	SwerveRotation2d GetGyroAngle();
	void AutonInitTask() override;
	void AutonEndTask() override;
	void TeleopInitTask() override;
	void PostLoopTask() override;
	void TeleopEndTask() override;
};