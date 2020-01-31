#pragma once

#include <iostream>

#include "COREFramework/COREScheduler.h"
#include "COREUtilities/CORETimer.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/COREHardwareManager.h"
#include <frc/WPILib.h>


namespace CORE {
    class CORERobot : public CORESubsystem
		      , public frc::TimedRobot
		      {
    public:
        CORERobot();
        ~CORERobot();

        void RobotInit();
		void RobotPeriodic();
		void DisabledInit();
		void DisabledPeriodic();
		void AutonomousInit();
		void AutonomousPeriodic();
		void TeleopInit();
		void TeleopPeriodic();
		void TestInit();
		void TestPeriodic();

    private:
        bool m_loopStarted = false;
        bool m_autonComplete = false;
    };
}
