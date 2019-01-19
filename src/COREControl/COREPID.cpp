#include "COREPID.h"

using namespace CORE;

COREPID::COREPID(double kP, double kI, double kD, double kF) : m_profile(kP, kI, kD, kF) {
    if(kF == 0) {
        CORELog::LogWarning("PID kF set to 0, this disables calculation!");
    }
    m_mistake = 0;
    m_lastMistake = 0;
    m_riemannSum = 0;
}

COREPID::COREPID(COREPID::PIDProfile &profile) {
    m_profile = profile;
}

void COREPID::SetProportionalConstant(double kP) {
    m_profile.kP = kP;
}

void COREPID::SetIntegralConstant(double kI) {
    m_profile.kI = kI;
}

void COREPID::SetDerivativeConstant(double kD) {
    m_profile.kD = kD;
}

void COREPID::SetFeedForwardConstant(double kF) {
    m_profile.kF = kF;
}

void COREPID::SetPIDProfile(PIDProfile &profile) {
    m_profile = profile;
}

double COREPID::Calculate(double mistake) {
    double time = m_timer.Get();
    if(time == 0) {
        m_timer.Reset();
        m_timer.Start();
        return 0;
    }
    double output = Calculate(mistake, time);
    m_timer.Reset();
    m_timer.Start();
    return output;
}

double COREPID::Calculate(double mistake, double dt) {
    if(dt == 0) {
        return 0;
    }
    m_proportional = mistake * m_profile.kP;
    m_riemannSum += mistake * dt;
    m_integral = m_riemannSum * m_profile.kI;
    m_derivative = ((mistake - m_lastMistake) / dt) * m_profile.kD;
    m_lastMistake = mistake;
    m_mistake = mistake;
    return m_profile.kF * (m_proportional + m_integral + m_derivative);
}

double COREPID::GetProportional() const {
    return m_proportional;
}

double COREPID::GetIntegral() const {
    return m_integral;
}

double COREPID::GetDerivative() const {
    return m_derivative;
}

double COREPID::GetMistake() const {
    return m_mistake;
}

double COREPID::GetProportionalConstant() {
    return m_profile.kP;
}

double COREPID::GetIntegralConstant() {
    return m_profile.kI;
}

double COREPID::GetDerivativeConstant() {
    return m_profile.kD;
}

double COREPID::GetFeedForwardConstant() {
    return m_profile.kF;
}

COREPositionPID::COREPositionPID(double kP, double kI, double kD, double kF) : COREPID(kP, kI, kD, kF) {

}

COREPositionPID::COREPositionPID(COREPID::PIDProfile &profile) : COREPID(profile) {

}

double COREPositionPID::Calculate(double actualPosition, double setPointPosition) {
    return COREPID::Calculate(setPointPosition - actualPosition);
}

double COREPositionPID::Calculate(double actualPosition, double setPointPosition, double dt) {
    return COREPID::Calculate(setPointPosition - actualPosition, dt);
}

COREAnglePID::COREAnglePID(double kP, double kI, double kD, double kF) : COREPID(kP, kI, kD, kF) {

}

COREAnglePID::COREAnglePID(COREPID::PIDProfile &profile) : COREPID(profile) {

}

double COREAnglePID::Calculate(COREVector actualAngle, COREVector setPointAngle) {
    return COREPID::Calculate(actualAngle.ShortestRotationTo(setPointAngle));
}

double COREAnglePID::Calculate(COREVector actualAngle, COREVector setPointAngle, double dt) {
    return COREPID::Calculate(actualAngle.ShortestRotationTo(setPointAngle), dt);
}
