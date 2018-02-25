#pragma once

#include <iostream>

#include "COREFramework/COREScheduler.h"
#include "COREUtilities/CORETimer.h"
//#include "COREDashboard/COREDriverstation.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/COREHardwareManager.h"

#ifndef NOT_REAL
#include <WPILib.h>
#endif


namespace CORE {
    class CORERobot : public CORESubsystem
#ifndef NOT_REAL 
		      , public TimedRobot 
#endif	      
		      {
    public:
        CORERobot();
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
        bool m_autonComplete = false;
        bool m_autonEnded = false;
    };
}
