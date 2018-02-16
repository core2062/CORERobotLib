#include "CORERobot.h"

using namespace CORE;

CORERobot::CORERobot() {

}

void CORERobot::StartCompetition() {
	COREDriverstation::updateRobotState();

}

void CORERobot::RobotInit() {
	COREDriverstation::updateRobotState();
	COREScheduler::robotInit();
}

void CORERobot::TeleopInit() {
	COREDriverstation::updateRobotState();
	COREScheduler::teleopInit();
}

void CORERobot::DisabledInit() {
	COREDriverstation::updateRobotState();
	COREScheduler::disabled();
}

void CORERobot::AutonomousInit() {
	COREDriverstation::updateRobotState();
	bool autonComplete = false;
	COREScheduler::autonInit();
	autonComplete = COREScheduler::auton();
}

void CORERobot::TestInit() {
	COREDriverstation::updateRobotState();
	//LiveWindow::GetInstance()->Run();
	COREScheduler::test();
}

void CORERobot::RobotPeriodic() {
	COREDriverstation::updateRobotState();
}

void CORERobot::DisabledPeriodic() {
}

void CORERobot::AutonomousPeriodic() {
	COREDriverstation::updateRobotState();
	bool autonComplete = false;
	if (IsAutonomous() && IsEnabled() && !autonComplete) {
		autonComplete = COREScheduler::auton();
	} else {
		COREScheduler::autonEnd();
	}
}

void CORERobot::TeleopPeriodic() {
	COREDriverstation::updateRobotState();
	COREScheduler::teleopInit();
	COREScheduler::teleop();
	COREScheduler::teleopEnd();
}

void CORERobot::TestPeriodic() {
	COREDriverstation::updateRobotState();
	COREScheduler::test();

}

CORERobot::~CORERobot() {
	CORELog::logInfo("Cleaning up CORERobot!");
	COREScheduler::cleanUp();
}
