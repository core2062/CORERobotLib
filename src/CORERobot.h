#pragma once

#include "WPILib.h"

#include "CORESubsystemsManager.h"
#include "COREHardware/CORETimer.h"

#define START_SIMULATED_ROBOT_CLASS(_ClassName_)                             \
	int main() {                                                             \
		_ClassName_ *robot = new _ClassName_();                              \
	    robot->OperatorControl();                                            \
	    return 0;                                                         	 \
	}                                                                        \


using namespace CORE;

namespace CORE {

class CORERobot : public CORESubsystem, public SampleRobot {
private:
    bool loopStarted = false;
    CORETimer loopTimer;
    const double TARGETLOOPTIME = 0.1;
public:
    CORERobot();
    void waitLoopTime();
	void RobotInit();
	void Disabled();
	void Autonomous();
	void OperatorControl();
	void Test();
};
}