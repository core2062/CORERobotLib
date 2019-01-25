#include "CORERobot.h"
#include <iostream>

using namespace CORE;

CORERobot::CORERobot() {
}

CORERobot::~CORERobot() {
	CORELog::LogInfo("Cleaning up CORERobot!");
	COREScheduler::CleanUp();
}

void CORERobot::RobotInit() {
	COREDriverstation::UpdateRobotState();
	COREScheduler::robotInit();
}

void CORERobot::RobotPeriodic() {
	COREScheduler::Log();
}

void CORERobot::DisabledInit() {
	COREDriverstation::UpdateRobotState();
	COREScheduler::Disabled();
}

void CORERobot::DisabledPeriodic() {
}

void CORERobot::AutonomousInit() {
	COREDriverstation::UpdateRobotState();
	m_autonComplete = false;
	COREScheduler::AutonInit();
}

void CORERobot::AutonomousPeriodic() {
	if (!m_autonComplete) {
		m_autonComplete = COREScheduler::Auton();
	}
}

void CORERobot::TeleopInit() {
	COREDriverstation::UpdateRobotState();
	COREScheduler::teleopInit();
}

void CORERobot::TeleopPeriodic() {
	COREScheduler::Teleop();
}

void CORERobot::TestInit() {
	COREDriverstation::UpdateRobotState();
	COREScheduler::testInit();
}

void CORERobot::TestPeriodic() {
	COREScheduler::Test();
}
