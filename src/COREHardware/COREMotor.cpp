#include "COREMotor.h"
#include <iostream>
#include <math.h>
#include "../COREHardware.h"

using namespace std;

using namespace CORE;

COREMotor::COREMotor(int port, controllerType controller, controlMode controlMethod, double pProfile1Value, double iProfile1Value, double dProfile1Value, double pProfile2Value, double iProfile2Value, double dProfile2Value, int integralAccuracy) :
	COREPID(motorControlMode == VELPID ? PIDType::VEL : (motorControlMode == POSPID ? PIDType::POS : PIDType::POSVEL), pProfile1Value, iProfile1Value, dProfile1Value, pProfile2Value, iProfile2Value, dProfile2Value, integralAccuracy),
	motorControlMode(controlMethod),
	motorControllerType(controller),
	motorPort(port)
{
#ifdef __arm__
	if(motorControllerType == CORE::CANTALON) {
		std::shared_ptr<CANTalon> pointer(new CANTalon(port));
		CANTalonController = pointer;
	}
	else if(motorControllerType == CORE::JAGUAR) {
		std::shared_ptr<Jaguar> pointer(new Jaguar(port));
		JaguarController = pointer;
	}
	else if(motorControllerType == CORE::VICTOR) {
		std::shared_ptr<Victor> pointer(new Victor(port));
		VictorController = pointer;
	}
	else {
		//TODO: Throw error
	}
#else
	trapSumTimer = new CORETimer();
	trapSumTimer->Reset();
	trapSumTimer->Start();
#endif
	std::shared_ptr<COREMotor> pointer(this);
	COREHardware::addMotor(pointer);
}

void COREMotor::Set(double motorSetValue) {
	motorValue = (reversed ? -motorSetValue : motorSetValue);
	motorUpdated = true;
}

double COREMotor::Get() {
	return motorValue;
}

void COREMotor::setReversed(bool reverse) {
	reversed = reverse;
}

bool COREMotor::getReversed() {
	return reversed;
}

void COREMotor::setControlMode(controlMode controlMethod) {
	motorControlMode = controlMethod;
	if(motorControlMode == POSPID) {
		setType(POS);
	}
	else if(motorControlMode == VELPID) {
		setType(VEL);
	}
}

controlMode COREMotor::getControlMode() {
	return motorControlMode;
}

int COREMotor::getPort() {
	return motorPort;
}

controllerType COREMotor::getControllerType() {
	return motorControllerType;
}

void COREMotor::setDeadband(double range) {
	setDeadband(-range, range);
}

void COREMotor::setDeadband(double min, double max) {
	deadBandMin = min < -1 ? -1 : min;
	deadBandMax = max > 1 ? 1 : max;
}

void COREMotor::addSlave(COREMotor *slaveMotor) {
	slaveMotors.push_back(slaveMotor);
}

void COREMotor::postTeleopTask() {
	//setActualPos(getActualPos());
	//setActualVel(getActualVel());
	if(motorControlMode == POSPID) {
		motorValue = calculate();
		motorUpdated = true;
	}
	else if(motorControlMode == VELPID) {
		motorValue = calculate();
		motorUpdated = true;
	}
	if(!motorUpdated) {
		motorValue = 0;
		cout << "Motor not updated!" << endl;
	}
	motorValue = fabs(motorValue) > 1 ? (motorValue > 1 ? 1 : -1) : motorValue;
	motorValue = (motorValue < deadBandMax && motorValue > deadBandMin) ? 0 : motorValue;
#ifdef __arm__
	for(auto motor : slaveMotors) {
		motor->Set(motorValue);
	}
	if(motorControllerType == CORE::CANTALON) {
		CANTalonController->Set(motorValue);
	}
	else if(motorControllerType == CORE::JAGUAR) {
		JaguarController->Set(motorValue);
	}
	else if(motorControllerType == CORE::VICTOR) {
		VictorController->Set(motorValue);
	}
	else {
		//TODO: Throw error
	}
#else
	cout << "Motor Port " << getPort() << " Value = " << motorValue << endl;
	trapSum -= 0.5 * trapSumTimer->Get() * (lastMotorValue + motorValue);
	trapSumTimer->Reset();
	lastMotorValue = motorValue;
	cout << "Trap Port " << getPort() << " Value = " << trapSum << endl;
#endif
	motorUpdated = false;
}
