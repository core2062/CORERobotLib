#include "COREHardware.h"

using namespace CORE;

vector<COREMotor*> COREHardwareManager::m_motors;
vector<COREEncoder*> COREHardwareManager::m_encoders;

void COREHardwareManager::addMotor(COREMotor* motor) {
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

void COREHardwareManager::addEncoder(COREEncoder* encoder) {
    m_encoders.push_back(encoder);
    if(!encoder->IsBoundToCANTalon()) {
        CORELog::logInfo("Encoder in ports " + to_string(encoder->GetPortA()) + ", "
                         + to_string(encoder->GetPortB()) + " added");
    } else {
        CORELog::logInfo("Encoder bound to CAN Talon ID: " + to_string(encoder->CANTalonController->GetDeviceID())
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
