#include "COREMotor.h"

using namespace CORE::COREHardware;

template<class T>
double COREMotor<T>::getEncoderValue() {
#ifdef NSIMULATION
	return motor->GetEncPosition();
#else
	//TODO:Do a trapezoidal sum
#endif
	return -1;
}

template<class T>
void COREMotor<T>::setControlMode(controlMode controlMethod) {
	motorControlMode = controlMethod;
}
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
