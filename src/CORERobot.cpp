#include "CORERobot.h"

using namespace CORE;

CORERobot::CORERobot() : CORESubsystem("Robot") {

}

void CORERobot::waitLoopTime() {
    if (!m_loopStarted) {
        m_loopTimer.Reset();
        m_loopTimer.Start();
        m_loopStarted = true;
    }
    double loopTime = m_loopTimer.Get() < m_targetLoopTime ? m_targetLoopTime - m_loopTimer.Get() : 0.0;
    //SmartDashboard::PutNumber("Timer", m_loopTimer.Get());
    if (m_loopTimer.Get() >= m_targetLoopTime * 1.2) {
        //outLog.appendLog("[PROBLEM] Loop Time High! Timer at: ", m_loopTimer.Get());
        cout << "Loop m_timer high at " << m_loopTimer.Get() << " seconds!" << endl;
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
    COREScheduler::disabled();
}

void CORERobot::Autonomous() {
    bool autonComplete = false;
    COREScheduler::autonInit();
    while(IsAutonomous() && IsEnabled() && !autonComplete) {
        autonComplete = COREScheduler::auton();
        waitLoopTime();
    }
}

void CORERobot::OperatorControl() {
    COREScheduler::teleopInit();
    while(IsOperatorControl() && IsEnabled()) {
        COREScheduler::teleop();
        waitLoopTime();
    }
    COREScheduler::teleopEnd();
}

void CORERobot::Test() {
    COREScheduler::test();
}
