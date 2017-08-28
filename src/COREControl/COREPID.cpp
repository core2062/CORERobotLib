#include "COREPID.h"

using namespace CORE;

COREPID::COREPID(double kP, double kI, double kD, double kF) : m_profile(kP, kI, kD, kF) {
    if(kF == 0) {
        CORELog::logWarning("PID kF set to 0, this disables calculation!");
    }
    m_lastMistake = 0;
    m_riemannSum = 0;
}

COREPID::COREPID(COREPID::PIDProfile &profile) {
    m_profile = profile;
}

void COREPID::setProportionalConstant(double kP) {
    m_profile.kP = kP;
}

void COREPID::setIntegralConstant(double kI) {
    m_profile.kI = kI;
}

void COREPID::setDerivativeConstant(double kD) {
    m_profile.kD = kD;
}

void COREPID::setFeedForwardConstant(double kF) {
    m_profile.kF = kF;
}

void COREPID::setPIDProfile(PIDProfile &profile) {
    m_profile = profile;
}

double COREPID::calculate(double mistake) {
    double time = m_timer.Get();
    if(time == 0) {
        m_timer.Reset();
        m_timer.Start();
        return 0;
    }
    double output = calculate(mistake, time);
    m_timer.Reset();
    m_timer.Start();
    return output;
}

double COREPID::calculate(double mistake, double dt) {
    if(dt == 0) {
        return 0;
    }
    m_proportional = mistake * m_profile.kP;
    m_riemannSum += m_proportional;
    double riemann = m_mistake * dt;
    m_integral = (riemann + m_riemannSum) * m_profile.kI;
    m_derivative = ((mistake - m_lastMistake) / dt) * m_profile.kD;
    m_lastMistake = mistake;
    m_mistake = mistake;
    double output = m_profile.kF * (m_proportional + m_integral + m_derivative);
    return output;
}

double COREPID::getProportional() const {
    return m_proportional;
}

double COREPID::getIntegral() const {
    return m_integral;
}

double COREPID::getDerivative() const {
    return m_derivative;
}

double COREPID::getMistake() const {
    return m_mistake;
}

COREPositionPID::COREPositionPID(double kP, double kI, double kD, double kF) : COREPID(kP, kI, kD, kF) {

}

COREPositionPID::COREPositionPID(COREPID::PIDProfile &profile) : COREPID(profile) {

}

double COREPositionPID::calculate(double actualPosition, double setPointPosition) {
    return COREPID::calculate(setPointPosition - actualPosition);
}

COREAnglePID::COREAnglePID(double kP, double kI, double kD, double kF) : COREPID(kP, kI, kD, kF) {

}

COREAnglePID::COREAnglePID(COREPID::PIDProfile &profile) : COREPID(profile) {

}

double COREAnglePID::calculate(Rotation2d actualAngle, Rotation2d setPointAngle) {
    double counterClockwiseMove = actualAngle.getCompassDegrees() - setPointAngle.getCompassDegrees();
    double clockwiseMove = setPointAngle.getCompassDegrees() - actualAngle.getCompassDegrees();
    clockwiseMove = clockwiseMove < 0 ? 360 + clockwiseMove : clockwiseMove;
    counterClockwiseMove = counterClockwiseMove < 0 ? 360 + counterClockwiseMove : counterClockwiseMove;
    return COREPID::calculate(clockwiseMove < counterClockwiseMove ? clockwiseMove : -counterClockwiseMove);
}
