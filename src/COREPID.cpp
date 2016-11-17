#include "COREPID.h"

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
		return getProfile(defaultProfile);
		break;
	}
}

/**
 * Initialize a PID loop with given P, I, and D values
 * @param pProfile1Value The P constant for profile 1, Set to 0 to disable
 * @param iProfile1Value The I constant for profile 1, Set to 0 to disable
 * @param dProfile1Value The D constant for profile 1, Set to 0 to disable
 * @param fProfile1Value The F constant for profile 1, Set to 1 or 0 to disable
 * @param pProfile2Value The P constant for profile 2, Set to 0 to disable. Disabled by default
 * @param iProfile2Value The I constant for profile 2, Set to 0 to disable. Disabled by default
 * @param dProfile2Value The D constant for profile 2, Set to 0 to disable. Disabled by default
 * @param fProfile2Value The F constant for profile 2, Set to 1 or 0 to disable. Disabled by default
 * @param integralAccuracy The number of previous errors to use when calculating the Integral term. Set to 1 by default
 */
COREPID::COREPID(PIDType PIDControllerType, double pProfile1Value, double iProfile1Value, double dProfile1Value, double fProfile1Value, double pProfile2Value, double iProfile2Value, double dProfile2Value, double fProfile2Value, int integralAccuracy) {
	PID1.P = pProfile1Value;
	PID1.I = iProfile1Value;
	PID1.D = dProfile1Value;
	PID1.F = fProfile1Value;
	PID2.P = pProfile2Value;
	PID2.I = iProfile2Value;
	PID2.D = dProfile2Value;
	PID2.F = fProfile2Value;
	for(int i = 1; i <= 2; i++) {
		getProfile(i)->mistake.resize(integralAccuracy);
		getProfile(i)->mistake[0] = 0;
		getProfile(i)->lastOutput = 0;
		getProfile(i)->proportional = 0;
		getProfile(i)->integral = 0;
		getProfile(i)->derivative = 0;
	}
	ControllerType = PIDControllerType;
	if(integralAccuracy < 1) {
		integralAccuracy = 1;
	}
	inputDevice = inputDeviceType::NoInput;
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
	double sum = 0;
	for(auto value : currentProfile->mistake) {
		sum += value;
	}
	double time = timer.Get();
	if(time == 0) {
		currentProfile->output = actualPosition;
		return 0;
	}
	if(currentProfile->F == 1) {
		currentProfile->F = 0;
	}
	else {
		timer.Reset();
		if(ControllerType == POS) {
			currentProfile->proportional = (setPosition - actualPosition) * currentProfile->P;
			currentProfile->mistake.insert(currentProfile->mistake.begin(), currentProfile->proportional);
			currentProfile->integral += (sum * time) * currentProfile->I; //Technically an approximation of Integral
			currentProfile->derivative = ((currentProfile->mistake[0] - currentProfile->mistake[1]) / time) * currentProfile->D;
			currentProfile->output = currentProfile->F * (currentProfile->proportional + currentProfile->integral + currentProfile->derivative);
		}
		else if(ControllerType == VEL) {
			currentProfile->proportional = (setVelocity - actualVelocity) * currentProfile->P;
			currentProfile->mistake.insert(currentProfile->mistake.begin(), currentProfile->proportional);
			currentProfile->integral += (sum * time) * currentProfile->I; //Technically an approximation of Integral
			currentProfile->derivative = ((currentProfile->mistake[0] - currentProfile->mistake[1]) / time) * currentProfile->D;
			currentProfile->output = currentProfile->F * (currentProfile->lastOutput + currentProfile->proportional + currentProfile->integral + currentProfile->derivative);
		}
		else {

		}
	}
	currentProfile->lastOutput = currentProfile->output;
	return currentProfile->output;
}

/**
 * Where you want the PID loop to target in its calculations
 * @param setPoint The new set point of the PID loop
 */
void COREPID::setPos(double setPointValue) {
	setPosition = setPointValue;
}

void COREPID::setVel(double setVelocityValue) {
	setVelocity = setVelocityValue;
}

/**
* Get the position which this PID loop was set to
*/
double COREPID::getPos() {
	return setPosition;
}

/**
* Get the velocity which this PID loop was set to
*/
double COREPID::getVel () {
	return setVelocity;
}

/**
 * The position of the mechanism controlled by the PID loop
 * @param actualPosition The actual position of the mechanism
 */
void COREPID::setActualPos(double actualPositionValue) {
	actualPosition = actualPositionValue;
}

void COREPID::setActualVel(double actualVelocityValue) {
	actualVelocity = actualVelocityValue;
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
 * Get the F value of this PID loop
 */
double COREPID::getF(int profile) {
	return getProfile(profile)->F;
}

void COREPID::setDefaultProfile(int profile) {
	defaultProfile = profile;
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
 * Set the D value of this PID loop
 */
void COREPID::setD(double value, int profile) {
	getProfile(profile)->D = value;
}

/**
 * Set the F value of this PID loop
 */
void COREPID::setF(double value, int profile) {
	getProfile(profile)->F = value;
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
double COREPID::getProportional(int profile) {
	return getProfile(profile)->proportional;
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

PIDType COREPID::getType() {
	return ControllerType;
}

void COREPID::setType(PIDType type) {
	ControllerType = type;
}

void COREPID::preTeleopTask() {
	switch(inputDevice) {
	case inputDeviceType::AHRSInput:
		//actualPosition = inputGyro->GetAngle();
		break;
	default:
		break;
	}
}

void COREPID::postTeleopTask() {
	calculate();
}
