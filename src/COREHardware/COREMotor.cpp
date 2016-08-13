#include "COREMotor.h"

using namespace CORE;

COREMotor::COREMotor(int port, controllerType controller, controlMode controlMethod, double pProfile1Value, double iProfile1Value, double dProfile1Value, double pProfile2Value, double iProfile2Value, double dProfile2Value, int integralAccuracy):
motorControlMode(controlMethod)
{
	if(!(pProfile1Value == 0 && iProfile1Value == 0 && dProfile1Value == 0 && pProfile2Value == 0 && iProfile2Value == 0 && dProfile2Value == 0) || controlMethod == VelPID || controlMethod == PosPID) {
		COREPID::PIDType PIDControllerType = motorControlMode == VelPID ? COREPID::PIDType::Vel : COREPID::PIDType::Pos;
		std::shared_ptr<COREPID> pointer(new COREPID(PIDControllerType, pProfile1Value, iProfile1Value, dProfile1Value, pProfile2Value, iProfile2Value, dProfile2Value, integralAccuracy));
		PIDController = pointer;
	}
#ifdef NSIMULATION
	motor = new T(port);
#else
	trapSumTimer = new CORETimer();
	trapSumTimer->Reset();
	trapSumTimer->Start();
#endif
}

void COREMotor::Set(double speed) {
	if(motorControlMode <= Current) {
		motorSpeed = speed;
	}
	else {
	}
}

double COREMotor::Get() {
	return motorValue;
}

void COREMotor::setControlMode(controlMode controlMethod) {
	motorControlMode = controlMethod;
}

controlMode COREMotor::getControlMode() {
	return motorControlMode;
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

void CORE::COREMotor::postTeleopTask()
{
	motorValue = abs(motorValue) > 1 ? (motorValue > 1 ? 1 : -1) : motorValue;
	motorValue = (motorValue < deadBandMax && motorValue > deadBandMin) ? 0 : motorValue;
#ifdef NSIMULATION
	for(auto motor : slaveMotors) {
	motor->Set(motorSpeed);
	}
	motor->Set(motorSpeed);
#else
	cout << "Motor Value = " << motorValue << endl;
	trapSum += 0.5 * trapSumTimer->Get() * (lastMotorValue + motorValue);
	trapSumTimer->Reset();
	lastMotorValue = motorValue;
	cout << "Trap Value = " << trapSum << endl;
#endif
	motorSpeed = 0;
}
//void postTeleopTask();

//template<class T>
//COREMotor<T>::controlMode COREMotor<T>::getControlMode() {
//	return motorControlMode;
//}

//template<class T>
//void COREMotor<T>::addSlave(COREMotor *slaveMotor) {
//	slaveMotors.push_back(slaveMotor);
//}
//
//template<>
//void COREMotor<CANTalon>::postTeleopTask() {
//#ifdef NSIMULATION
//	for(auto motor : slaveMotors) {
//		motor->Set(motorSpeed);
//	}
//	motor->Set(motorSpeed);
//#endif
//	motorSpeed = 0;
//}
//
//template<>
//void COREMotor<Jaguar>::postTeleopTask() {
//#ifdef NSIMULATION
//	for(auto motor : slaveMotors) {
//		motor->Set(motorSpeed);
//	}
//	motor->Set(motorSpeed);
//#endif
//	motorSpeed = 0;
//}
//}
//}
	motorUpdated = false;
}