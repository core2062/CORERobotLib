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

class SwerveTracker /*: public CORETask*/ {
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
    IMotorController *m_steer = nullptr;
    IMotorController *m_drive = nullptr;
    AHRS *m_gyro = nullptr;
    thread *m_mainLoop = nullptr;
    static SwerveTracker *m_instance;
    CORESwerve *m_swerveDrive = nullptr;

    SwerveTracker();

    void addData(double time, Position2d data, Position2d::Delta vel);

    COREDataLogger log;

public:
    bool doLog = false;

    COREConstant <double> m_fudgeFactor;
    static SwerveTracker *GetInstance();

    void init(TalonSRX *steer, TalonSRX *drive, AHRS *gyro);

    ~SwerveTracker();
    void injectCORESwerve(CORESwerve * swerveDrive);

    void reset(double time, Position2d initial);

    void loop();

    void start();

    void stop();

    Position2d getFieldToVehicle(double time);

    Position2d getLatestFieldToVehicle();

    COREVector generateOdometry();

    std::pair<double, double> getEncoderInches();

    std::pair<double, double> getEncoderSpeed();

    COREVector getGyroAngle();

    void autonInitTask() ;

    void autonEndTask() ;

    void teleopInitTask() ;

    void postLoopTask() ;

    void teleopEndTask() ;
};

