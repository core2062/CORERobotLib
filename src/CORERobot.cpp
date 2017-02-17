#include "CORERobot.h"

using namespace CORE;

gameMode CORERobot::m_mode = DISABLE;

CORERobot::CORERobot() : CORESubsystem("Robot") {

}

void CORERobot::waitLoopTime() {
    if (!m_loopStarted) {
        m_loopTimer.Reset();
        m_loopTimer.Start();
        m_loopStarted = true;
    }
    double loopTime = m_loopTimer.Get() < m_targetLoopTime ? m_targetLoopTime - m_loopTimer.Get() : 0.0;
    if (m_loopTimer.Get() >= m_targetLoopTime * 1.2) {
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
    COREScheduler::robotInit();
}

void CORERobot::Disabled() {
	m_mode = DISABLE;
    COREScheduler::disabled();
}

void CORERobot::Autonomous() {
	m_mode = AUTO;
    bool autonComplete = false;
    COREScheduler::autonInit();
    while(IsAutonomous() && IsEnabled() && !autonComplete) {
        autonComplete = COREScheduler::auton();
        waitLoopTime();
    }
}

void CORERobot::OperatorControl() {
	m_mode = TELE;
    COREScheduler::teleopInit();
    while(IsOperatorControl() && IsEnabled()) {
        COREScheduler::teleop();
        waitLoopTime();
    }
    COREScheduler::teleopEnd();
}

void CORERobot::Test() {
	m_mode = TEST;
    while(IsEnabled()) {
        COREScheduler::test();
        waitLoopTime();
    }
}

gameMode CORERobot::getMode(){
	return m_mode;
}
