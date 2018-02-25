#include "CORERobot.h"
#include <iostream>

using namespace CORE;

CORERobot::CORERobot() {

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
	m_autonComplete = false;
    m_autonEnded = false;
	COREScheduler::autonInit();
}

void CORERobot::TestInit() {
	COREDriverstation::updateRobotState();
	//LiveWindow::GetInstance()->Run();
	COREScheduler::test();
}

void CORERobot::RobotPeriodic() {

}

void CORERobot::DisabledPeriodic() {

}

void CORERobot::AutonomousPeriodic() {
	if (!m_autonComplete) {
		m_autonComplete = COREScheduler::auton();
	} else if (!m_autonEnded) {
        m_autonEnded = false;
		COREScheduler::autonEnd();
	}
}

void CORERobot::TeleopPeriodic() {
	COREScheduler::teleop();
}

void CORERobot::TestPeriodic() {
	COREScheduler::test();
}

CORERobot::~CORERobot() {
	CORELog::logInfo("Cleaning up CORERobot!");
	COREScheduler::cleanUp();
}
