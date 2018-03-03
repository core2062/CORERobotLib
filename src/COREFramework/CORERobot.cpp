#include "CORERobot.h"
#include <iostream>

using namespace CORE;

CORERobot::CORERobot() {
}

CORERobot::~CORERobot() {
	CORELog::logInfo("Cleaning up CORERobot!");
	COREScheduler::cleanUp();
}

void CORERobot::RobotInit() {
	COREDriverstation::updateRobotState();
	COREScheduler::robotInit();
}

void CORERobot::RobotPeriodic() {
}

void CORERobot::DisabledInit() {
	COREDriverstation::updateRobotState();
	COREScheduler::disabled();
}

void CORERobot::DisabledPeriodic() {
}

void CORERobot::AutonomousInit() {
	COREDriverstation::updateRobotState();
	m_autonComplete = false;
	COREScheduler::autonInit();
}

void CORERobot::AutonomousPeriodic() {
	if (!m_autonComplete) {
		m_autonComplete = COREScheduler::auton();
	}
}

void CORERobot::TeleopInit() {
	COREDriverstation::updateRobotState();
	COREScheduler::teleopInit();
}

void CORERobot::TeleopPeriodic() {
	COREScheduler::teleop();
}

void CORERobot::TestInit() {
	COREDriverstation::updateRobotState();
	COREScheduler::testInit();
}

void CORERobot::TestPeriodic() {
	COREScheduler::test();
}
