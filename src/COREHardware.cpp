#include "COREHardware.h"

using namespace CORE;


std::map<int, std::shared_ptr<COREMotor>> Robot::m_motorMap;
std::map<int, std::shared_ptr<COREJoystick>> Robot::m_joystickMap;

void Robot::addMotor(shared_ptr<COREMotor> motor) {
	if (m_motorMap.count(motor->getPort()) == 1) {
		//TODO: Error: motor in port # already registered!
		cout << "Motor in port: " << motor->getPort() << " already added!" << endl;
	}
	else {
		m_motorMap[motor->getPort()] = motor;
		cout << "Motor port: " << motor->getPort() << " added" << endl;
	}
}

void Robot::addJoystick(shared_ptr<COREJoystick> joystick) {
	if (m_joystickMap.count(joystick->getPort()) == 1) {
		//TODO: Error: joystick in port # already registered!
	}
	else {
		m_joystickMap[joystick->getPort()] = joystick;
	}
}

shared_ptr<COREMotor> Robot::motor(int port) {
	if (m_motorMap.count(port) == 1) {
		//TODO: Error: motor in port # not found!
		return m_motorMap[port];
	}
	else {
		return m_motorMap[port];
	}
}

shared_ptr<COREJoystick> Robot::joystick(int port) {
	if (m_joystickMap.count(port) == 1) {
		//TODO: Error: joystick in port # not found!
		return m_joystickMap[port];
	}
	else {
		return m_joystickMap[port];
	}
}

void Robot::disableAllMotors() {
	for (auto motor : m_motorMap) {
		motor.second->Set(0);
	}
}
