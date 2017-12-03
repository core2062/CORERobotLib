#pragma once

#include "COREUtilities/Position2d.h"
#include "TankKinematics.h"
#include "InterpolatingMap.h"
#include "CANTalon.h"
#include "AHRS.h"
#include "../COREUtilities/CORETimer.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/CORETask.h"
#include "WPILib.h"
#include <thread>
#include "CORELogging/COREDataLog.h"

using namespace CORE;

class TankTracker : public CORETask {
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
    CANTalon *m_left = nullptr;
    CANTalon *m_right = nullptr;
    AHRS *m_gyro = nullptr;
    thread *m_mainLoop = nullptr;
    static TankTracker *m_instance;

    TankTracker();

    void start();

    void stop();

    void addData(double time, Position2d data, Position2d::Delta vel);

    COREDataLogger log;

public:
    bool doLog = false;

    static TankTracker *GetInstance();

    void init(CANTalon *left, CANTalon *right, AHRS *gyro);

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
