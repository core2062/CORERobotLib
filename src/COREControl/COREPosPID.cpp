#include "COREPosPID.h"

using namespace CORE;

COREPosPID::COREPosPID(double kP, double kI, double kD, double kF) {
    m_kP = kP;
    m_kI = kI;
    m_kD = kD;
    m_kF = kF;
    if(kF == 0) {
        CORELog::logWarning("PID kF set to 0, this disables calculation!");
    }
    m_lastMistake = 0;
    m_riemannSum = 0;
}

void COREPosPID::setKp(double kP) {
    m_kP = kP;
}

void COREPosPID::setKi(double kI) {
    m_kI = kI;
}

void COREPosPID::setKd(double kD) {
    m_kD = kD;
}

double COREPosPID::calculate() {
    double time = timer.Get();
    if(time == 0) {
        timer.Reset();
        timer.Start();
        return 0;
    }
    m_mistake = m_setPoint - m_actualPosition;
    m_proportional = m_mistake * m_kP;
    m_riemannSum += m_proportional;
    double riemann = m_mistake * time;
    m_integral = (riemann + m_riemannSum) * m_kI;
    m_derivative = ((m_mistake - m_lastMistake) / time) * m_kD;
    m_lastMistake = m_mistake;
    double output = m_kF * (m_proportional + m_integral + m_derivative);
    timer.Reset();
    timer.Start();
    return output;
}

double COREPosPID::getProportional() const {
    return m_proportional;
}

double COREPosPID::getIntegral() const {
    return m_integral;
}

double COREPosPID::getDerivative() const {
    return m_derivative;
}

double COREPosPID::getMistake() {
    return m_mistake;
}
