#pragma once

#include "COREUtilities/Position2d.h"
#include "COREDrive/CORESwerveDrive.h"
#include "InterpolatingMap.h"
#include "ctre/Phoenix.h"
#include "AHRS.h"
#include "../COREUtilities/CORETimer.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/CORETask.h"
#include "WPILib.h"
#include <thread>
#include "CORELogging/COREDataLog.h"
#include "COREDrive/CORESwerveDrive.h"
#include "COREUtilities/COREConstant.h"

using namespace CORE;

class SwerveTracker : public CORETask {
protected:
    InterpolatingTreeMap m_data;
    Position2d::Delta m_velocity = {0, 0, 0};

private:
    const double m_targetLoopHz = 200; //If this is changed, be sure to adjust NavX constructor accordingly
    double m_targetLoopTime = 0.01;
    atomic<bool> m_loopEnabled;
    mutex m_loopLock;
    mutex m_dataLock;
    mutex m_timerLock;
    double m_leftPrev = 0;
    double m_rightPrev = 0;
    CORETimer m_loopTimer;
    IMotorController *m_left = nullptr;
    IMotorController *m_right = nullptr;
    AHRS *m_gyro = nullptr;
    thread *m_mainLoop = nullptr;
    static SwerveTracker *m_instance;
    void injectCORESwerve(CORESwerve * swerveDrive);
    CORESwerve *m_swerveDrive = nullptr;

    SwerveTracker();

    void start();

    void stop();

    void addData(double time, Position2d data, Position2d::Delta vel);

    COREDataLogger log;

public:
    bool doLog = false;

    COREConstant <double> m_fudgeFactor;
    static SwerveTracker *GetInstance();

    void init(TalonSRX *left, TalonSRX *right, AHRS *gyro);

    ~SwerveTracker();

    void reset(double time, Position2d initial);

    void loop();

    Position2d getFieldToVehicle(double time);

    Position2d getLatestFieldToVehicle();

    Position2d generateOdometry(COREVector heading);

    std::pair<double, double> getEncoderInches();

    std::pair<double, double> getEncoderSpeed();

    COREVector getGyroAngle();

    void autonInitTask() override;

    void autonEndTask() override;

    void teleopInitTask() override;

    void postLoopTask() override;

    void teleopEndTask() override;
};
