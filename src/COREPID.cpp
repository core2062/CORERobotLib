#include "COREPID.h"

using namespace CORE;

/**
 * Returns the pointer to the struct of the profile number given
 * @param profile The profile number to get
 * @return A pointer to the PIDProfile struct
 */
COREPID::PIDProfile::PIDMode* COREPID::getPIDMode(PIDType pidType, int profile) {
    switch(profile) {
        case 1:
            if(pidType == PIDType::POS) {
                return &m_PID1.pos;
            } else if(pidType == PIDType::VEL) {
                return &m_PID1.vel;
            } else if(pidType == PIDType::ANG) {
                return &m_PID1.ang;
            } else {
                cout << "Error! PID type invalid!" << endl;
            }
        case 2:
            if(pidType == PIDType::POS) {
                return &m_PID2.pos;
            } else if(pidType == PIDType::VEL) {
                return &m_PID2.vel;
            } else if(pidType == PIDType::ANG) {
                return &m_PID2.ang;
            } else {
                cout << "Error! PID type invalid!" << endl;
            }
        default:
            return getPIDMode(pidType, m_defaultProfile);
    }
}

COREPID::PIDProfile* COREPID::getPIDProfile(int profile) {
    switch(profile) {
        case 1:
            return &m_PID1;
        case 2:
            return &m_PID2;
        default:
            return getPIDProfile(m_defaultProfile);
    }
}

double COREPID::posPID(double setPoint) {
    double sum = 0;
    for(auto value : m_currentProfile->pos.riemannSum) {
        sum += value;
    }
    m_currentProfile->pos.mistake = setPoint - m_pos.actual;
    m_currentProfile->pos.proportional = m_currentProfile->pos.mistake * m_currentProfile->pos.P;
    m_currentProfile->pos.riemannSum.insert(m_currentProfile->pos.riemannSum.begin(),
                                            m_currentProfile->pos.proportional * m_time);
    double riemann = m_currentProfile->pos.mistake * m_time;
    m_currentProfile->pos.integral = (riemann + sum) * m_currentProfile->pos.I;
    m_currentProfile->pos.riemannSum.pop_back();
    m_currentProfile->pos.riemannSum.insert(m_currentProfile->pos.riemannSum.begin(), riemann);
    m_currentProfile->pos.derivative = ((m_currentProfile->pos.mistake - m_currentProfile->pos.lastMistake) / m_time)
                                       * m_currentProfile->pos.D;
    m_currentProfile->pos.lastMistake = m_currentProfile->pos.mistake;
    m_currentProfile->pos.output = m_currentProfile->pos.F * (m_currentProfile->pos.proportional
                                                              + m_currentProfile->pos.integral
                                                              + m_currentProfile->pos.derivative);
    return m_currentProfile->pos.output;
}

double COREPID::velPID(double setPoint) {
    double sum = 0;
    for(auto value : m_currentProfile->vel.riemannSum) {
        sum += value;
    }
    m_currentProfile->vel.mistake = setPoint - m_vel.actual;
    m_currentProfile->vel.proportional = m_currentProfile->vel.mistake * m_currentProfile->vel.P;
    m_currentProfile->vel.riemannSum.insert(m_currentProfile->vel.riemannSum.begin(),
                                            m_currentProfile->vel.proportional * m_time);
    double riemann = m_currentProfile->vel.mistake * m_time;
    m_currentProfile->vel.integral = (riemann + sum) * m_currentProfile->vel.I;
    m_currentProfile->vel.riemannSum.pop_back();
    m_currentProfile->vel.riemannSum.insert(m_currentProfile->vel.riemannSum.begin(), riemann);
    m_currentProfile->vel.derivative = ((m_currentProfile->vel.mistake - m_currentProfile->vel.lastMistake) / m_time)
                                       * m_currentProfile->vel.D;
    m_currentProfile->vel.lastMistake = m_currentProfile->vel.mistake;
    m_currentProfile->vel.output = m_currentProfile->vel.F * (m_currentProfile->vel.lastOutput
                                                              + m_currentProfile->vel.proportional
                                                              + m_currentProfile->vel.integral
                                                              + m_currentProfile->vel.derivative);
    m_currentProfile->vel.lastOutput = m_currentProfile->vel.output;
    return m_currentProfile->vel.output;
}

double COREPID::angPID(double setPoint) {
    double sum = 0;
    for(auto value : m_currentProfile->ang.riemannSum) {
        sum += value;
    }
    m_currentProfile->ang.mistake = (setPoint - m_ang.actual * m_ticksToDegrees)
                                    - (360 * floor(0.5 + (setPoint - m_ang.actual * m_ticksToDegrees) / 360));
    m_currentProfile->ang.proportional = m_currentProfile->ang.mistake * m_currentProfile->ang.P;
    m_currentProfile->ang.riemannSum.insert(m_currentProfile->ang.riemannSum.begin(),
                                            m_currentProfile->ang.proportional * m_time);
    double riemann = m_currentProfile->ang.mistake * m_time;
    m_currentProfile->ang.integral = (riemann + sum) * m_currentProfile->ang.I;
    m_currentProfile->ang.riemannSum.pop_back();
    m_currentProfile->ang.riemannSum.insert(m_currentProfile->ang.riemannSum.begin(), riemann);
    m_currentProfile->ang.derivative = ((m_currentProfile->ang.mistake - m_currentProfile->ang.lastMistake) / m_time)
                                       * m_currentProfile->ang.D;
    m_currentProfile->ang.lastMistake = m_currentProfile->ang.mistake;
    m_currentProfile->ang.output = m_currentProfile->ang.F * (m_currentProfile->ang.proportional
                                                              + m_currentProfile->ang.integral
                                                              + m_currentProfile->ang.derivative);
    return m_currentProfile->ang.output;
}

/**
 * Initialize a PID loop with given P, I, D, and F values
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
COREPID::COREPID(ControllerInput* inputDevice, ControllerOutput* outputDevice, PIDType pidType,
                 double pProfile1Value, double iProfile1Value, double dProfile1Value, double fProfile1Value,
                 double pProfile2Value, double iProfile2Value, double dProfile2Value, double fProfile2Value,
                 int integralAccuracy) {
    m_inputDevice = inputDevice;
    m_outputDevice = outputDevice;
    for(auto type : m_PIDTypes) {
        getPIDMode(type, 1)->P = pProfile1Value;
        getPIDMode(type, 1)->I = iProfile1Value;
        getPIDMode(type, 1)->D = dProfile1Value;
        getPIDMode(type, 1)->F = fProfile1Value;
        getPIDMode(type, 2)->P = pProfile2Value;
        getPIDMode(type, 2)->I = iProfile2Value;
        getPIDMode(type, 2)->D = dProfile2Value;
        getPIDMode(type, 2)->F = fProfile2Value;
    }
    for(int i = 1; i <= 2; i++) {
        for(auto type : m_PIDTypes) {
            getPIDMode(type, i)->riemannSum.resize(integralAccuracy);
            getPIDMode(type, i)->riemannSum[0] = 0;
            getPIDMode(type, i)->lastOutput = 0;
            getPIDMode(type, i)->proportional = 0;
            getPIDMode(type, i)->integral = 0;
            getPIDMode(type, i)->derivative = 0;
            getPIDMode(type, i)->mistake = 0;
            getPIDMode(type, i)->lastMistake = 0;
        }
    }
    if(integralAccuracy < 1) {
        integralAccuracy = 1;
    }
    m_timer.Reset();
    m_timer.Start();
}

/**
 * Initialize a PID loop with given P, I, D, and F values
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
COREPID::COREPID(double pProfile1Value, double iProfile1Value, double dProfile1Value, double fProfile1Value,
                 double pProfile2Value, double iProfile2Value, double dProfile2Value, double fProfile2Value,
                 int integralAccuracy) {
    m_inputDevice = nullptr;
    m_outputDevice = nullptr;
    for(auto type : m_PIDTypes) {
        getPIDMode(type, 1)->P = pProfile1Value;
        getPIDMode(type, 1)->I = iProfile1Value;
        getPIDMode(type, 1)->D = dProfile1Value;
        getPIDMode(type, 1)->F = fProfile1Value;
        getPIDMode(type, 2)->P = pProfile2Value;
        getPIDMode(type, 2)->I = iProfile2Value;
        getPIDMode(type, 2)->D = dProfile2Value;
        getPIDMode(type, 2)->F = fProfile2Value;
    }
    for(int i = 1; i <= 2; i++) {
        for(auto type : m_PIDTypes) {
            getPIDMode(type, i)->riemannSum.resize(integralAccuracy);
            getPIDMode(type, i)->riemannSum[0] = 0;
            getPIDMode(type, i)->lastOutput = 0;
            getPIDMode(type, i)->proportional = 0;
            getPIDMode(type, i)->integral = 0;
            getPIDMode(type, i)->derivative = 0;
            getPIDMode(type, i)->mistake = 0;
            getPIDMode(type, i)->lastMistake = 0;
        }
    }
    if(integralAccuracy < 1) {
        integralAccuracy = 1;
    }
    m_timer.Reset();
    m_timer.Start();
}

/**
 * Calculate the output of the PID loop
 * @param profile The profile number to use
 * @return The output of the PID loop
 */
double COREPID::calculate(int profile) {
    m_currentProfile = getPIDProfile(profile);
    m_time = m_timer.Get();
    m_timer.Reset();
    m_timer.Start();
    if(m_time == 0) {
        for(auto type : m_PIDTypes) {
            getPIDMode(type, profile)->output = 0;
        }
        return 0;
    }
    for(auto type : m_PIDTypes) {
        if(getPIDMode(type, profile)->F == 0) {
            getPIDMode(type, profile)->F = 1;
        }
    }
    if(m_pos.enabled && m_vel.enabled && !m_ang.enabled) { //POS_VEL
        m_pos.enabled = false;
        m_vel.enabled = false;
        return velPID(posPID(m_pos.setPoint));
    } else if(!m_pos.enabled && m_vel.enabled && m_ang.enabled) { //ANG_VEL
        m_ang.enabled = false;
        m_vel.enabled = false;
        return velPID(angPID(m_ang.setPoint));
    } else if(m_pos.enabled && !m_vel.enabled && !m_ang.enabled) { //POS
        m_pos.enabled = false;
        return posPID(m_pos.setPoint);
    } else if(!m_pos.enabled && m_vel.enabled && !m_ang.enabled) { //VEL
        m_vel.enabled = false;
        return velPID(m_vel.setPoint);
    } else if(!m_pos.enabled && !m_vel.enabled && m_ang.enabled) { //ANG
        m_ang.enabled = false;
        return angPID(m_ang.setPoint);
    } else if(!m_pos.enabled && !m_vel.enabled && !m_ang.enabled) { //None
        CORELog::logWarning("PID not fed!");
        return 0;
    } else {
        m_pos.enabled = false;
        m_vel.enabled = false;
        m_ang.enabled = false;
        CORELog::logError("Illegal PID configuration!");
        return 0;
    }
}

/**
 * Where you want the PID loop to target in its calculations
 * @param positionSetPoint The new position set point of the PID loop
 */
void COREPID::setPos(double positionSetPoint) {
    m_pos.setPoint = positionSetPoint;
}

void COREPID::setVel(double velocitySetPoint) {
    m_vel.setPoint = velocitySetPoint;
}

void COREPID::setAng(double angleSetPoint) {
    m_ang.setPoint = angleSetPoint;
}

/**
* Get the position which this PID loop was set to
*/
double COREPID::getPos() {
    return m_pos.setPoint;
}

/**
* Get the velocity which this PID loop was set to
*/
double COREPID::getVel() {
    return m_vel.setPoint;
}

/**
* Get the velocity which this PID loop was set to
*/
double COREPID::getAng() {
    return m_ang.setPoint;
}

/**
 * The position of the mechanism controlled by the PID loop
 * @param actualPos The actual position of the mechanism
 */
void COREPID::setActualPos(double actualPos) {
    m_pos.actual = actualPos;
}

void COREPID::setActualVel(double actualVel) {
    m_vel.actual = actualVel;
}

void COREPID::setActualAng(double actualAng) {
    m_ang.actual = actualAng;
}

/**
 * Get the P value of this PID loop
 */
double COREPID::getP(PIDType pidType, int profile) {
    return getPIDMode(pidType, profile)->P;
}

/**
 * Get the I value of this PID loop
 */
double COREPID::getI(PIDType pidType, int profile) {
    return getPIDMode(pidType, profile)->I;
}

/**
 * Get the D value of this PID loop
 */
double COREPID::getD(PIDType pidType, int profile) {
    return getPIDMode(pidType, profile)->D;
}

/**
 * Get the F value of this PID loop
 */
double COREPID::getF(PIDType pidType, int profile) {
    return getPIDMode(pidType, profile)->F;
}

void COREPID::setDefaultProfile(int profile) {
    m_defaultProfile = profile;
}

/**
 * Set the P value of this PID loop
 */
void COREPID::setP(double value, PIDType pidType, int profile) {
    getPIDMode(pidType, profile)->P = value;
}

/**
 * Set the I value of this PID loop
 */
void COREPID::setI(double value, PIDType pidType, int profile) {
    getPIDMode(pidType, profile)->I = value;
}

/**
 * Set the D value of this PID loop
 */
void COREPID::setD(double value, PIDType pidType, int profile) {
    getPIDMode(pidType, profile)->D = value;
}

/**
 * Set the F value of this PID loop
 */
void COREPID::setF(double value, PIDType pidType, int profile) {
    getPIDMode(pidType, profile)->F = value;
}

/**
 * Get the output of this PID loop
 */
double COREPID::getOutput(PIDType pidType, int profile) {
    return getPIDMode(pidType, profile)->output;
}

/**
 * Get the proportional term of this PID loop
 */
double COREPID::getProportional(PIDType pidType, int profile) {
    return getPIDMode(pidType, profile)->proportional;
}

/**
 * Get the integral term of this PID loop
 */
double COREPID::getIntegral(PIDType pidType, int profile) {
    return getPIDMode(pidType, profile)->integral;
}

/**
 * Get the derivative term of this PID loop
 */
double COREPID::getDerivative(PIDType pidType, int profile) {
    return getPIDMode(pidType, profile)->derivative;
}

void COREPID::preLoopTask() {
    if(m_inputDevice != nullptr) {
        switch(m_pidType) {
            case POS:
                setActualPos(m_inputDevice->ControllerGetPos());
                break;
            case VEL:
                setActualVel(m_inputDevice->ControllerGetVel());
                break;
            case ANG:
                setActualAng(m_inputDevice->ControllerGetAng());
                break;
            case POS_VEL:
                setActualPos(m_inputDevice->ControllerGetPos());
                setActualVel(m_inputDevice->ControllerGetVel());
                break;
            case ANG_VEL:
                setActualAng(m_inputDevice->ControllerGetAng());
                setActualVel(m_inputDevice->ControllerGetVel());
                break;
        }
    }
}

void COREPID::postLoopTask() {
    m_outputDevice->ControllerSet(calculate());
}
