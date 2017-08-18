#pragma once

#include <WPILib.h>
#include <iostream>

#include "COREScheduler.h"
#include "COREUtilities/CORETimer.h"
#include "CORELogging/CORELog.h"

#define START_SIMULATED_ROBOT_CLASS(_ClassName_)                             \
	int main() {                                                             \
		_ClassName_ *robot = new _ClassName_();                              \
        robot->RobotInit();                                                  \
		robot->Autonomous();                                                 \
	    robot->OperatorControl();                                            \
	    return 0;                                                         	 \
	}                                                                        \

namespace CORE {
    class CORERobot : public CORESubsystem, public SampleRobot {
    public:
        CORERobot();
        void waitLoopTime();
        void setLoopTime(double loopTime);
        void RobotInit();
        void Disabled();
        virtual void autonInit(){};
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
