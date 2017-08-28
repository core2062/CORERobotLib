#include "CORESimRobot.h"

using namespace CORE::SIM;

CORESimRobot::CORESimRobot() {
    COREDriverstation::updateRobotState();
    COREScheduler::robotInit();
}

void CORESimRobot::OperatorControl() {
    COREScheduler::teleopInit();
    while(true) {
        COREScheduler::teleop();
        Wait(m_targetLoopTime);
    }
    COREScheduler::teleopEnd();
}

void CORESimRobot::Autonomous() {
    COREDriverstation::updateRobotState();
    bool autonComplete = false;
    COREScheduler::autonInit();
    while(!autonComplete) {
        //COREHardwareManager::zeroMotors();
        autonComplete = COREScheduler::auton();
        Wait(m_targetLoopTime);
    }
}

CORESimRobot::~CORESimRobot() {
    COREScheduler::cleanUp();
}
