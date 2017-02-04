#include "COREHardware.h"

using namespace CORE;

vector<COREMotor*> COREHardwareManager::m_motors;

void COREHardwareManager::addMotor(COREMotor * motor) {
    m_motors.push_back(motor);
    CORELog::logInfo("Motor in port " + to_string(motor->getPort()) + "Added");
}

void COREHardwareManager::updateMotors() {
    for(auto motor : m_motors) {
        motor->update();
    }
}