#pragma once

#include <iostream>

#include "COREFramework/COREScheduler.h"
#include "COREUtilities/CORETimer.h"
//#include "COREUtilities/COREDriverstation.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/COREHardwareManager.h"

#include <WPILib.h>


namespace CORE {
    class CORERobot : public CORESubsystem, public TimedRobot {
    public:
        CORERobot();
        void StartCompetition();
        void RobotInit();
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
    };
}
