#include "CORERobot.h"
#include <iostream>

using namespace CORE;

CORERobot::CORERobot() {
	std::cout<<"CORERobot Constructor"<<endl;
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
	COREScheduler::log();
}

void CORERobot::DisabledInit() {
	COREDriverstation::UpdateRobotState();
	COREScheduler::disabled();
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
	COREScheduler::teleop();
}

void CORERobot::TestInit() {
	COREDriverstation::UpdateRobotState();
	COREScheduler::testInit();
}

void CORERobot::TestPeriodic() {
	COREScheduler::test();
}
