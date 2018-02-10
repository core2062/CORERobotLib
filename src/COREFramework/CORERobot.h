#pragma once

#include <iostream>

#include "COREFramework/COREScheduler.h"
#include "COREUtilities/CORETimer.h"
//#include "COREDashboard/COREDriverstation.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/COREHardwareManager.h"

#include <WPILib.h>


namespace CORE {
    class CORERobot : public CORESubsystem, public TimedRobot {
    public:
        CORERobot();
        void waitLoopTime();
        void StartCompetition();
        void SetPeriod(double seconds);
        void RobotInit();
        void setLoopTime(double loopTime);
        void DisabledInit();
        void AutonomousInit();
        void TeleopInit();
        void TestInit();

        void RobotPeriodic();
        void DisabledPeriodic();
        void AutonomousPeriodic();
        void TeleopPeriodic();
        void TestPeriodic();
        ~CORERobot();
    private:
        bool m_loopStarted = false;
        CORETimer m_loopTimer;
        double m_targetLoopTime = 0.01;
    };
}
