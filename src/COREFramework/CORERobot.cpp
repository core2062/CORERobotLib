#include "CORERobot.h"

using namespace CORE;

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
	COREDriverstation::updateRobotState();
    COREScheduler::robotInit();
}

void CORERobot::Disabled() {
    COREDriverstation::updateRobotState();
    COREScheduler::disabled();
}

void CORERobot::Autonomous() {
    COREDriverstation::updateRobotState();
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
    COREDriverstation::updateRobotState();
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
    COREDriverstation::updateRobotState();
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
