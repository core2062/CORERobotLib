#pragma once

#include "TankPosition2d.h"
#include "TankKinematics.h"
#include "TankInterpolatingMap.h"
#include <ctre/phoenix.h>
#include "AHRS.h"
#include "COREUtilities/CORETimer.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/CORETask.h"
#include "CORELogging/COREDataLog.h"
#include <frc/WPILib.h>
#include <thread>
#include <mutex>
// #include "COREDataLog.h"

using namespace CORE;

class TankTracker : public CORETask {
protected:
	TankInterpolatingTreeMap m_data;
	TankPosition2d::TankDelta m_velocity = {0,0,0};

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
	std::thread * m_mainLoop = nullptr;
	static TankTracker * m_instance;
	TankTracker();
	void Start();
    void Stop();
	void AddData(double time, TankPosition2d data, TankPosition2d::TankDelta vel);
	COREDataLogger log;

public:
	bool doLog = false;
	static TankTracker* GetInstance();
	void Init(TalonSRX * left, TalonSRX * right, AHRS * gyro);
	~TankTracker();
	void Reset(double time, TankPosition2d initial);
	void Loop();
	TankPosition2d GetFieldToVehicle(double time);
	TankPosition2d GetLatestFieldToVehicle();
	TankPosition2d GenerateOdometry(double leftDelta, double rightDelta, TankRotation2d heading);
	std::pair<double, double> GetEncoderInches();
	std::pair<double, double> GetEncoderSpeed();
	TankRotation2d GetGyroAngle();
	void AutonInitTask() override;
	void AutonEndTask() override;
	void TeleopInitTask() override;
	void PostLoopTask() override;
	void TeleopEndTask() override;
};
