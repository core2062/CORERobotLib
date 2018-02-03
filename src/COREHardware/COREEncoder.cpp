//#include "COREEncoder.h"
//#include "COREFramework/COREHardwareManager.h"
//
//using namespace CORE;
//
//COREEncoder::COREEncoder(int portA, int portB, encoderType encoderType, bool reversed) : m_encoderType(encoderType),
//                                                                                         m_reversed(reversed) {
//    shared_ptr<Encoder> pointer(new Encoder(portA, portB));
//    encoder = pointer;
//    m_portA = portA;
//    m_portB = portB;
//    m_boundToTalonSRX = false;
//    Reset();
//    COREHardwareManager::addEncoder(this);
//}
//
//COREEncoder::COREEncoder(shared_ptr<TalonSRX> boundTalonSRXController, encoderType encoder, bool reversed) :
//        m_encoderType(encoder),
//        m_reversed(reversed) {
//    TalonSRXController = boundTalonSRXController;
//    m_portA = -1;
//    m_portB = -1;
//    m_boundToTalonSRX = true;
//    Reset();
//    COREHardwareManager::addEncoder(this);
//}
//
//void COREEncoder::setEncReversed(bool reversed) {
//    m_reversed = reversed;
//}
//
//bool COREEncoder::getEncReversed() {
//    return m_reversed;
//}
//
//int COREEncoder::GetEncPos() {
//    return m_pos;
//}
//
//double COREEncoder::GetEncVel() {
//    return m_vel;
//}
//
//double COREEncoder::GetEncAccel() {
//    return m_accel;
//}
//
//double COREEncoder::getDistance(distanceUnit unit) {
//    return m_ticksToDistanceConversion[unit];
//}
//
//void COREEncoder::setDistance(distanceUnit unit, double ticksToUnit) {
//    m_ticksToDistanceConversion[unit] = ticksToUnit;
//}
//
//void COREEncoder::SetEncPos(int value) {
//    if(m_boundToTalonSRX) {
//        TalonSRXController->SetEncPosition(value);
//        m_offset = 0;
//    } else {
//        encoder->Reset();
//        m_offset = value;
//    }
//    m_lastPos = value;
//    m_lastVel = 0;
//}
//
//void COREEncoder::Reset() {
//    SetEncPos(0);
//}
//
//int COREEncoder::getPortA() {
//    return m_portA;
//}
//
//int COREEncoder::getPortB() {
//    return m_portB;
//}
//
//bool COREEncoder::isBoundToTalonSRX() {
//    return m_boundToTalonSRX;
//}
//
//void COREEncoder::Update() {
//    double time = m_timer.Get();
//    if(time != 0) {
//        if(m_boundToTalonSRX) {
//            m_pos = TalonSRXController->GetEncPosition();
//            m_vel = TalonSRXController->GetEncVel();
//        } else {
//            m_pos = (encoder->Get() * (m_reversed ? -1 : 1)) + m_offset;
//            m_vel = (m_pos - m_lastPos) / time;
//        }
//        m_accel = (m_vel - m_lastVel) / time;
//        m_lastPos = m_pos;
//        m_lastVel = m_vel;
//    }
//    m_timer.Reset();
//    m_timer.Start();
//}
//
//int COREEncoder::getTalonSRXID() {
//    return TalonSRXController->GetDeviceID();
//}
