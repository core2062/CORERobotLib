#pragma once

#include <WPILib.h>
#include <iostream>

#include "COREScheduler.h"
#include "COREHardware/CORETimer.h"
#include "CORELog.h"

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
        enum gameMode{
        	TELEOP,
        	AUTON,
        	TEST,
        	DISABLE
        };

        enum gameAlliance {
        	RED = -1,
        	BLUE = 1,
        	INVALID = 0
        };
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
        static gameMode getMode();
        static gameAlliance getAlliance();
        static int getStation();
        static bool IsCompetition();
    private:
        bool m_loopStarted = false;
        CORETimer m_loopTimer;
        double m_targetLoopTime = 0.01;
        static gameMode m_mode;
        static gameAlliance m_alliance;
        static int m_station;
        static bool m_isCompetition;
        void updateRobotState();
    };
}
