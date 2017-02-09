#include "COREHardware.h"

using namespace CORE;

vector<COREMotor*> COREHardwareManager::m_motors;

void COREHardwareManager::addMotor(COREMotor * motor) {
	bool error = false;
	int port = motor->getPort();
    for(auto motor : m_motors) {
    	if(motor->getPort() == port) {
    		CORELog::logError("Cannot add two motors of port " + to_string(port) + " to robot!");
    	}
    }
    if(!error) {
        m_motors.push_back(motor);
        CORELog::logInfo("Motor in port " + to_string(motor->getPort()) + " added");
    }
}

void COREHardwareManager::zeroMotors() {
	for(auto motor : m_motors) {
		motor->Set(0);
	}
}

void COREHardwareManager::updateMotors() {
    for(auto motor : m_motors) {
        motor->update();
    }
}
