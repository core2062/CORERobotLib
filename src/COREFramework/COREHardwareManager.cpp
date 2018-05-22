#include "COREHardwareManager.h"

using namespace CORE;

vector<ICOREMotor*> COREHardwareManager::m_motors;
vector<ICOREEncoder*> COREHardwareManager::m_encoders;

void COREHardwareManager::AddMotor(ICOREMotor* motor) {
    bool error = false;
    int port = motor->GetPort();
    for(auto motor : m_motors) {
        if(motor->GetPort() == port) {
            CORELog::LogError("Cannot add two motors of port " + to_string(port) + " to robot!");
        }
    }
    if(!error) {
        m_motors.push_back(motor);
        CORELog::LogInfo("Motor in port " + to_string(motor->GetPort()) + " added");
    }
}

void COREHardwareManager::ZeroMotors() {
    for(auto motor : m_motors) {
        motor->Set(0);
    }
}

void COREHardwareManager::UpdateMotors() {
    for(auto motor : m_motors) {
        motor->Update();
    }
}

void COREHardwareManager::UpdateEncoders() {
    for(auto encoder : m_encoders) {
        encoder->Update();
    }
}

void COREHardwareManager::ZeroEncoders() {
    for(auto encoder : m_encoders) {
        encoder->Reset();
    }
}

void COREHardwareManager::AddEncoder(ICOREEncoder* encoder) {
    m_encoders.push_back(encoder);
    if(!encoder->IsBoundToTalonSRX()) {
        CORELog::LogInfo("Encoder in ports " + to_string(encoder->GetPortA()) + ", "
                         + to_string(encoder->GetPortB()) + " added");
    } else {
        CORELog::LogInfo("Encoder bound to CAN Talon ID: " + to_string(encoder->GetTalonSRXID())
                         + " added");
    }
}

void COREHardwareManager::CleanUp() {
	CORELog::LogInfo("Cleaning up COREHardwareManager!");
	for (auto i = m_motors.begin(); i != m_motors.end(); i++){
	    delete *i;
	}
	m_motors.clear();
	for (auto i = m_encoders.begin(); i != m_encoders.end(); i++){
		delete *i;
	}
	m_encoders.clear();
}

void COREHardwareManager::PreLoopTask() {
    ZeroMotors();
}
