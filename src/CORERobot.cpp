#include "CORERobot.h"

using namespace CORE;

CORERobot::gameMode CORERobot::m_mode = CORERobot::DISABLE;
CORERobot::gameAlliance CORERobot::m_alliance = CORERobot::INVALID;
int CORERobot::m_station = 0;
bool CORERobot::m_isCompetition = false;

void CORERobot::updateRobotState() {
	if(DriverStation::GetInstance().IsDisabled()) {
		m_mode = DISABLE;
	} else if(DriverStation::GetInstance().IsAutonomous()) {
		m_mode = AUTON;
	} else if(DriverStation::GetInstance().IsOperatorControl()) {
		m_mode = TELEOP;
	} else if(DriverStation::GetInstance().IsTest()) {
		m_mode = TEST;
	} else {
		m_mode = DISABLE;
	}

	if(DriverStation::GetInstance().GetAlliance() == DriverStation::kRed) {
		m_alliance = RED;
	} else if(DriverStation::GetInstance().GetAlliance() == DriverStation::kBlue) {
		m_alliance = BLUE;
	} else if(DriverStation::GetInstance().GetAlliance() == DriverStation::kInvalid) {
		m_alliance = INVALID;
	}

	m_station = DriverStation::GetInstance().GetLocation();
	m_isCompetition = DriverStation::GetInstance().IsFMSAttached();
}

CORERobot::CORERobot() : CORESubsystem("Robot") {

}

void CORERobot::waitLoopTime() {
    if(!m_loopStarted) {
        m_loopTimer.Reset();
        m_loopTimer.Start();
        m_loopStarted = true;
    }
    double loopTime = m_loopTimer.Get() < m_targetLoopTime ? m_targetLoopTime - m_loopTimer.Get() : 0.0;
    if(m_loopTimer.Get() >= m_targetLoopTime * 1.2) {
        CORELog::logWarning("Loop timer high at " + to_string(m_loopTimer.Get()) + " seconds!");
    }
    Wait(loopTime);
    m_loopTimer.Reset();
    m_loopTimer.Start();
}

void CORERobot::setLoopTime(double loopTime) {
    m_targetLoopTime = loopTime;
}

void CORERobot::RobotInit() {
	updateRobotState();
    COREScheduler::robotInit();
}

void CORERobot::Disabled() {
	updateRobotState();
    COREScheduler::disabled();
}

void CORERobot::Autonomous() {
	updateRobotState();
	autonInit();
    bool autonComplete = false;
    COREScheduler::autonInit();
    m_loopTimer.Reset();
    m_loopTimer.Start();
    while(IsAutonomous() && IsEnabled() && !autonComplete) {
        COREHardwareManager::zeroMotors();
        autonComplete = COREScheduler::auton();
        waitLoopTime();
    }
}

void CORERobot::OperatorControl() {
	updateRobotState();
    COREScheduler::teleopInit();
    m_loopTimer.Reset();
    m_loopTimer.Start();
    while(IsOperatorControl() && IsEnabled()) {
        COREScheduler::teleop();
        waitLoopTime();
    }
    COREScheduler::teleopEnd();
}

void CORERobot::Test() {
	updateRobotState();
	m_loopTimer.Reset();
	m_loopTimer.Start();
    while(IsEnabled()) {
        LiveWindow::GetInstance()->Run();
        COREScheduler::test();
        waitLoopTime();
    }
}

CORERobot::~CORERobot() {
	CORELog::logInfo("Cleaning up CORERobot!");
	COREScheduler::cleanUp();
}

CORERobot::gameMode CORERobot::getMode(){
	return m_mode;
}

CORERobot::gameAlliance CORERobot::getAlliance() {
	return m_alliance;
}

int CORERobot::getStation() {
	return m_station;
}

bool CORERobot::IsCompetition() {
	return m_isCompetition;
}
