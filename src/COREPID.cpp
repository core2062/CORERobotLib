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
		return &m_PID1;
		break;
	case 2:
		return &m_PID2;
		break;
	default:
		return getProfile(m_defaultProfile);
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
	m_PID1.P = pProfile1Value;
	m_PID1.I = iProfile1Value;
	m_PID1.D = dProfile1Value;
	m_PID1.F = fProfile1Value;
	m_PID2.P = pProfile2Value;
	m_PID2.I = iProfile2Value;
	m_PID2.D = dProfile2Value;
	m_PID2.F = fProfile2Value;
	for(int i = 1; i <= 2; i++) {
		getProfile(i)->mistake.resize(integralAccuracy);
		getProfile(i)->mistake[0] = 0;
		getProfile(i)->lastOutput = 0;
		getProfile(i)->proportional = 0;
		getProfile(i)->integral = 0;
		getProfile(i)->derivative = 0;
	}
	m_controllerType = PIDControllerType;
	if(integralAccuracy < 1) {
		integralAccuracy = 1;
	}
	m_inputDevice = inputDeviceType::NO_INPUT;
	m_timer.Reset();
	m_timer.Start();
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
	double time = m_timer.Get();
	if(time == 0) {
		currentProfile->output = m_actualPosition;
		return 0;
	}
	if(currentProfile->F == 1) {
		currentProfile->F = 0;
	}
	else {
		m_timer.Reset();
		if (m_controllerType == POS) {
			currentProfile->proportional = (m_setPosition - m_actualPosition) * currentProfile->P;
			currentProfile->mistake.insert(currentProfile->mistake.begin(), currentProfile->proportional);
			currentProfile->integral += (sum * time) * currentProfile->I; //Technically an approximation of Integral
			currentProfile->derivative = ((currentProfile->mistake[0] - currentProfile->mistake[1]) / time) * currentProfile->D;
			currentProfile->output = currentProfile->F * (currentProfile->proportional + currentProfile->integral + currentProfile->derivative);
		} else if (m_controllerType == VEL) {
			currentProfile->proportional = (m_setVelocity - m_actualVelocity) * currentProfile->P;
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
	m_setPosition = setPointValue;
}

void COREPID::setVel(double setVelocityValue) {
	m_setVelocity = setVelocityValue;
}

/**
* Get the position which this PID loop was set to
*/
double COREPID::getPos() {
	return m_setPosition;
}

/**
* Get the velocity which this PID loop was set to
*/
double COREPID::getVel () {
	return m_setVelocity;
}

/**
 * The position of the mechanism controlled by the PID loop
 * @param actualPosition The actual position of the mechanism
 */
void COREPID::setActualPos(double actualPositionValue) {
	m_actualPosition = actualPositionValue;
}

void COREPID::setActualVel(double actualVelocityValue) {
	m_actualVelocity = actualVelocityValue;
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
	m_defaultProfile = profile;
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
	return m_controllerType;
}

void COREPID::setType(PIDType type) {
	m_controllerType = type;
}

void COREPID::preTeleopTask() {
	switch (m_inputDevice) {
		case inputDeviceType::AHRS_INPUT:
			//m_actualPosition = m_inputGyro->GetAngle();
		break;
	default:
		break;
	}
}

void COREPID::postTeleopTask() {
	calculate();
}
