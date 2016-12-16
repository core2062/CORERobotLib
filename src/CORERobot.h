#pragma once

#include "WPILib.h"
#include <iostream>

#include "COREScheduler.h"
#include "CORESubsystem.h"
#include "COREHardware/CORETimer.h"

#define START_SIMULATED_ROBOT_CLASS(_ClassName_)                             \
	int main() {                                                             \
		_ClassName_ *robot = new _ClassName_();                              \
        robot->RobotInit();                                                  \
		robot->Autonomous();                                                 \
	    robot->OperatorControl();                                            \
	    return 0;                                                         	 \
	}                                                                        \


using namespace CORE;

namespace CORE {
    class CORERobot : public CORESubsystem, public SampleRobot {
    private:
        bool m_loopStarted = false;
        CORETimer m_loopTimer;
        double m_targetLoopTime = 0.1;
    public:
        CORERobot();
        void waitLoopTime();
        void setLoopTime(double loopTime);
        void RobotInit();
        void Disabled();
        void Autonomous();
        void OperatorControl();
        void Test();
    };
}