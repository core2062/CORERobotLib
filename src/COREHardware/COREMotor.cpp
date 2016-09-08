#include "COREMotor.h"
#include <iostream>
#include <math.h>
using namespace std;

using namespace CORE;

COREMotor::COREMotor(int port, controllerType controller, controlMode controlMethod, double pProfile1Value, double iProfile1Value, double dProfile1Value, double pProfile2Value, double iProfile2Value, double dProfile2Value, int integralAccuracy) :
	COREPID(motorControlMode == VELPID ? PIDType::VEL : (motorControlMode == POSPID ? PIDType::POS : PIDType::POSVEL), pProfile1Value, iProfile1Value, dProfile1Value, pProfile2Value, iProfile2Value, dProfile2Value, integralAccuracy),
	motorControlMode(controlMethod),
	motorControllerType(controller),
	motorPort(port)
{
#ifdef NSIMULATION
	switch(motorControllerType)
	{
	case CORE::CANTALON:
		std::shared_ptr<CANTalon> pointer(new CANTalon(port));
		CANTalonController = pointer;
		break;
	case CORE::JAGUAR:
		std::shared_ptr<Jaguar> pointer(new Jaguar(port));
		JaguarController = pointer;
		break;
	case CORE::VICTOR:
		std::shared_ptr<Victor> pointer(new Victor(port));
		VictorController = pointer;
		break;
	}
#else
	trapSumTimer = new CORETimer();
	trapSumTimer->Reset();
	trapSumTimer->Start();
#endif
}

void COREMotor::Set(double motorSetValue) {
	motorValue = (reversed ? -motorSetValue : motorSetValue);
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
#ifdef NSIMULATION
	for(auto motor : slaveMotors) {
		motor->Set(motorValue);
	}
	switch(motorControllerType)
	{
	case CORE::CANTALON:
		CANTalonController->Set(motorValue);
		break;
	case CORE::JAGUAR:
		JaguarController->Set(motorValue);
		break;
	case CORE::VICTOR:
		VictorController->Set(motorValue);
		break;
	}
#else
	cout << "Motor Value = " << motorValue << endl;
	trapSum += 0.5 * trapSumTimer->Get() * (lastMotorValue + motorValue);
	trapSumTimer->Reset();
	lastMotorValue = motorValue;
	cout << "Trap Value = " << trapSum << endl;
#endif
	motorUpdated = false;
}
