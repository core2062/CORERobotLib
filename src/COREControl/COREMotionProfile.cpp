#include "COREMotionProfile.h"

CORE::COREMotionProfile::COREMotionProfile(shared_ptr<ControllerInput> inputDevice, shared_ptr<ControllerOutput> outputDevice,
                                           double maxVel, double maxAccel) : m_actualVelocity(0), m_maxOutputSpeed(0),
                                                                             m_timeToAccel(0), m_ticksToAccel(0) {
    m_inputDevice = inputDevice;
    m_outputDevice = outputDevice;
    m_maxVel = maxVel;
    m_maxAccel = maxAccel;
    m_timer.Reset();
    m_timer.Start();
}

CORE::COREMotionProfile::COREMotionProfile(CORE::ControllerInput* inputDevice, CORE::ControllerOutput* outputDevice,
                                           double maxVel, double maxAccel) : m_actualVelocity(0), m_maxOutputSpeed(0),
                                                                             m_timeToAccel(0), m_ticksToAccel(0) {
	shared_ptr<ControllerInput> inputDevicePointer(inputDevice);
	shared_ptr<ControllerOutput> outputDevicePointer(outputDevice);
	COREMotionProfile(inputDevice, outputDevice, maxVel, maxAccel);
}

CORE::COREMotionProfile::COREMotionProfile(double maxVel, double maxAccel) : m_actualVelocity(0), m_maxOutputSpeed(0),
                                                                             m_timeToAccel(0), m_ticksToAccel(0) {
    m_inputDevice = nullptr;
    m_outputDevice = nullptr;
    m_maxVel = maxVel;
    m_maxAccel = maxAccel;
    m_timer.Reset();
    m_timer.Start();
}

void CORE::COREMotionProfile::SetMaxVel(double maxVelocity) {
    m_maxVel = maxVelocity;
}

void CORE::COREMotionProfile::SetMaxAccel(double maxAcceleration) {
    m_maxAccel = maxAcceleration;
}

double CORE::COREMotionProfile::GetMaxVel() {
    return m_maxVel;
}

double CORE::COREMotionProfile::GetMaxAccel() {
    return m_maxAccel;
}

void CORE::COREMotionProfile::Set(double setPoint) {
    COREMotionController::Set(setPoint);
    //m_actualVelocity = m_inputDevice->ControllerGetVel();
    m_timeToAccel = (m_maxVel - m_actualVelocity) / (m_maxAccel == 0 ? 1 : m_maxAccel);
    m_ticksToAccel = m_actualPosition + 0.5 * m_maxAccel * (m_timeToAccel * m_timeToAccel);
    m_maxOutputSpeed = 1;
    if((m_ticksToAccel * 2) > (m_setPoint - m_actualPosition)) {
        m_maxOutputSpeed = (m_ticksToAccel == 0 ? 0 : ((m_setPoint - m_actualPosition) * 0.5) / m_ticksToAccel);
    }
    m_timer.Reset();
    m_timer.Start();
}

double CORE::COREMotionProfile::Get() {
    return m_output;
}

void CORE::COREMotionProfile::SetActual(double actualPosition) {
    COREMotionController::SetActual(actualPosition);
}

double CORE::COREMotionProfile::GetActual() {
    return COREMotionController::GetActual();
}

void CORE::COREMotionProfile::update() {
    if(m_actualPosition < m_ticksToAccel) {
        //m_outputDevice->ControllerSet(m_timeToAccel == 0 ? 0 : (m_timer.Get() / m_timeToAccel));
        m_output = (m_timeToAccel == 0 ? 0 : (m_timer.Get() / m_timeToAccel));
    } else if(m_actualPosition > (m_setPoint - m_ticksToAccel)) {
        //m_outputDevice->ControllerSet(m_maxOutputSpeed - (m_timeToAccel == 0 ? 0 : (m_timer.Get() / m_timeToAccel)));
        m_output = (m_maxOutputSpeed - (m_timeToAccel == 0 ? 0 : (m_timer.Get() / m_timeToAccel)));
    } else {
        //m_outputDevice->ControllerSet(m_maxOutputSpeed);
        m_output = m_maxOutputSpeed;
        m_timer.Reset();
        m_timer.Start();
    }
}
