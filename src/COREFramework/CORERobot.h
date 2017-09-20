#pragma once

#include <iostream>

#include "COREFramework/COREScheduler.h"
#include "COREUtilities/CORETimer.h"
#include "COREDashboard/COREDriverstation.h"
#include "CORELogging/CORELog.h"
#include "COREFramework/COREHardwareManager.h"

#include <WPILib.h>


namespace CORE {
    class CORERobot : public CORESubsystem, public SampleRobot {
    public:
        CORERobot();
        void waitLoopTime();
        void setLoopTime(double loopTime);
        void RobotInit();
        void Disabled();
        void Autonomous();
        void OperatorControl();
        void Test();
        ~CORERobot();
    private:
        bool m_loopStarted = false;
        CORETimer m_loopTimer;
        double m_targetLoopTime = 0.01;
    };
}
