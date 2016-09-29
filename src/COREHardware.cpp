#include "COREHardware.h"

using namespace CORE;


std::map<int, std::shared_ptr<COREMotor>> Robot::motorMap;
std::map<int, std::shared_ptr<COREJoystick>> Robot::joystickMap;

void Robot::addMotor(shared_ptr<COREMotor> motor) {
	if(motorMap.count(motor->getPort()) == 1) {
		//TODO: Error: motor in port # already registered!
		cout << "Motor in port: " << motor->getPort() << " already added!" << endl;
	}
	else {
		motorMap[motor->getPort()] = motor;
		cout << "Motor port: " << motor->getPort() << " added" << endl;
	}
}

void Robot::addJoystick(shared_ptr<COREJoystick> joystick) {
	if(joystickMap.count(joystick->getPort()) == 1) {
		//TODO: Error: joystick in port # already registered!
	}
	else {
		joystickMap[joystick->getPort()] = joystick;
	}
}

shared_ptr<COREMotor> Robot::motor(int port) {
	if(motorMap.count(port) == 1) {
		//TODO: Error: motor in port # not found!
		return motorMap[port];
	}
	else {
		return motorMap[port];
	}
}

shared_ptr<COREJoystick> Robot::joystick(int port) {
	if(joystickMap.count(port) == 1) {
		//TODO: Error: joystick in port # not found!
		return joystickMap[port];
	}
	else {
		return joystickMap[port];
	}
}

void Robot::disableAllMotors() {
	for(auto motor : motorMap) {
		motor.second->Set(0);
	}
}
