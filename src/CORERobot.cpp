#include "CORERobot.h"

using namespace CORE;

CORERobot::CORERobot() {

}

void CORERobot::waitLoopTime() {
    if (!m_loopStarted) {
        m_loopTimer.Reset();
        m_loopTimer.Start();
        m_loopStarted = true;
    }
    double loopTime = m_loopTimer.Get() < m_TARGET_LOOP_TIME ? m_TARGET_LOOP_TIME - m_loopTimer.Get() : 0.0;
    //SmartDashboard::PutNumber("Timer", m_loopTimer.Get());
    if (m_loopTimer.Get() >= m_TARGET_LOOP_TIME * 1.2) {
        //outLog.appendLog("[PROBLEM] Loop Time High! Timer at: ", m_loopTimer.Get());
    }
    Wait(loopTime);
    m_loopTimer.Reset();
    m_loopTimer.Start();
}

void CORERobot::RobotInit() {
    CORESubsystemsManager::robotInit();
}

void CORERobot::Disabled() {

}

void CORERobot::Autonomous() {

}

void CORERobot::OperatorControl() {
    CORESubsystemsManager::teleopInit();
    while (IsOperatorControl() && IsEnabled()) {
        CORESubsystemsManager::teleop();
        waitLoopTime();
    }
    CORESubsystemsManager::teleopEnd();
}

void CORERobot::Test() {
    CORESubsystemsManager::test();
}

