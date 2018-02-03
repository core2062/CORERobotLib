#include "COREHardwareManager.h"

using namespace CORE;

vector<ICOREMotor*> COREHardwareManager::m_motors;
vector<ICOREEncoder*> COREHardwareManager::m_encoders;

void COREHardwareManager::addMotor(ICOREMotor* motor) {
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
        motor->Update();
    }
}

void COREHardwareManager::updateEncoders() {
    for(auto encoder : m_encoders) {
        encoder->Update();
    }
}

void COREHardwareManager::zeroEncoders() {
    for(auto encoder : m_encoders) {
        encoder->Reset();
    }
}

void COREHardwareManager::addEncoder(ICOREEncoder* encoder) {
    m_encoders.push_back(encoder);
    if(!encoder->isBoundToTalonSRX()) {
        CORELog::logInfo("Encoder in ports " + to_string(encoder->getPortA()) + ", "
                         + to_string(encoder->getPortB()) + " added");
    } else {
        CORELog::logInfo("Encoder bound to CAN Talon ID: " + to_string(encoder->getTalonSRXID())
                         + " added");
    }
}

void COREHardwareManager::cleanUp() {
	CORELog::logInfo("Cleaning up COREHardwareManager!");
	for (auto i = m_motors.begin(); i != m_motors.end(); i++){
	    delete *i;
	}
	m_motors.clear();
	for (auto i = m_encoders.begin(); i != m_encoders.end(); i++){
		delete *i;
	}
	m_encoders.clear();
}

void COREHardwareManager::preLoopTask() {
    zeroMotors();
}
