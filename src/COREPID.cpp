#include "COREPID.h"
#include "COREHardware.h"

using namespace CORE;

/**
 * Returns the pointer to the struct of the profile number given
 * @param profile The profile number to get
 * @return A pointer to the PIDProfile struct
 */
COREPID::PIDProfile *COREPID::getProfile(int profile) {
	switch(profile) {
	case 1:
		return &PID1;
		break;
	case 2:
		return &PID2;
		break;
	default:
		return &PID1;
		break;
	}
}

/**
 * Initialize a PID loop with given P, I, and D values
 * @param pProfile1Value The P constant for profile 1, Set to 0 to disable
 * @param iProfile1Value The I constant for profile 1, Set to 0 to disable
 * @param dProfile1Value The D constant for profile 1, Set to 0 to disable
 * @param pProfile2Value The P constant for profile 2, Set to 0 to disable. Disabled by default
 * @param iProfile2Value The I constant for profile 2, Set to 0 to disable. Disabled by default
 * @param dProfile2Value The D constant for profile 2, Set to 0 to disable. Disabled by default
 * @param integralAccuracy The number of previous errors to use when calculating the Integral term. Set to 1 by default
 */
COREPID::COREPID(PIDType PIDControllerType, double pProfile1Value, double iProfile1Value, double dProfile1Value, double pProfile2Value, double iProfile2Value, double dProfile2Value, int integralAccuracy) {
	PID1.P = pProfile1Value;
	PID1.I = iProfile1Value;
	PID1.D = dProfile1Value;
	PID2.P = pProfile2Value;
	PID2.I = iProfile2Value;
	PID2.D = dProfile2Value;
	ControllerType = PIDControllerType;
	if(integralAccuracy < 1) {
		integralAccuracy = 1;
	}
	PID1.mistake.reserve(integralAccuracy);
	PID1.mistake[1] = 0;
	PID2.mistake.reserve(integralAccuracy);
	PID2.mistake[1] = 0;
	inputDevice = inputDeviceType::NoInput;
	outputDevice = outputDeviceType::NoOutput;
	timer.Reset();
	timer.Start();
}

/**
 * Calculate the output of the PID loop
 * @param profile The profile number to use
 * @return The output of the PID loop
 */
double COREPID::calculate(int profile) {
	PIDProfile *currentProfile = getProfile(profile);
	switch(inputDevice) {
	case inputDeviceType::AHRSInput:
		//actualPosition = inputGyro->GetAngle();
		break;
	default:
		break;
	}
	if (ControllerType == Pos) {
		currentProfile->porportional = (setPointValue - actualPosition) * currentProfile->P;
		currentProfile->mistake.insert(currentProfile->mistake.begin(), currentProfile->porportional);
		double sum = 0;
		for(int i = 1; i < (int) currentProfile->mistake.size(); i++) {
			sum += currentProfile->mistake[i];
		}
		double time = timer.Get();
		if(time == 0) {
			currentProfile->output = actualPosition;
			return 0;
		}
		else {
			currentProfile->integral += (sum * time) * currentProfile->I; //Technically an approximation of Integral
			currentProfile->derivative = ((currentProfile->mistake[0] - currentProfile->mistake[1]) / time) * currentProfile->D;
			timer.Reset();
			timer.Start();
			currentProfile->output = currentProfile->porportional + currentProfile->integral + currentProfile->derivative;
			return currentProfile->output;
		}
	}
	else {
		return -1;
	}
}

/**
 * Calculate the output of the PID loop
 * @param newSetpoint The new set point of the PID loop
 * @param profile The profile number to use
 * @return The output of the PID loop
 */
double COREPID::calculate(double newSetpoint, int profile) {
	actualPosition = newSetpoint;
	return calculate(profile);
}

/**
 * Where you want the PID loop to target in its calculations
 * @param setPoint The new set point of the PID loop
 */
void COREPID::setPoint(double setPoint) {
	setPointValue = setPoint;
}

/**
 * The position of the mechanism controlled by the PID loop
 * @param actualPosition The actual position of the mechanism
 */
void COREPID::setActualPosition(double actualPositionValue) {
	actualPosition = actualPositionValue;
}

///**
// * Bind the pointer to a NAVX as an input device to this PID loop
// * @param NAVX A pointer to the NAVX
// */
//void COREPID::bindInputDevice(COREAHRS * NAVX) {
//	inputDevice = inputDeviceType::AHRSInput;
//	inputGyro = NAVX;
//}

/**
 * Get the position which this PID loop was set to
 */
double COREPID::getSetPoint() {
	return setPointValue;
}

/**
 * Get the P value of this PID loop
 */
double COREPID::getP(int profile) {
	return getProfile(profile)->P;
}

/**
 * Get the I value of this PID loop
 */
double COREPID::getI(int profile) {
	return getProfile(profile)->I;
}

/**
 * Get the D value of this PID loop
 */
double COREPID::getD(int profile) {
	return getProfile(profile)->D;
}

/**
 * Set the P value of this PID loop
 */
void COREPID::setP(double value, int profile) {
	getProfile(profile)->P = value;
}

/**
 * Set the I value of this PID loop
 */
void COREPID::setI(double value, int profile) {
	getProfile(profile)->I = value;
}

/**
 * Get the D value of this PID loop
 */
void COREPID::setD(double value, int profile) {
	getProfile(profile)->D = value;
}

/**
 * Get the output of this PID loop
 */
double COREPID::getOutput(int profile) {
	return getProfile(profile)->output;
}

/**
 * Get the proportional term of this PID loop
 */
double COREPID::getPorportional(int profile) {
	return getProfile(profile)->porportional;
}

/**
 * Get the integral term of this PID loop
 */
double COREPID::getIntegral(int profile) {
	return getProfile(profile)->integral;
}

/**
 * Get the derivative term of this PID loop
 */
double COREPID::getDerivative(int profile) {
	return getProfile(profile)->derivative;
}
