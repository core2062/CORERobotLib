#include "COREDrive.h"

using namespace CORE;

COREDrive::COREDrive() {
    PreLoopTask();
}

void COREDrive::SetRotation(double rotation) {
    m_rot = rotation;
}

double COREDrive::GetRotation() {
    return m_rot;
}

void COREDrive::SetThottle(double throttle) {
    m_throttle = throttle;
}

double COREDrive::GetThrottle() {
    return m_throttle;
}

void COREDrive::Cartesian(double x, double y, double rotation) {
    m_x = x;
    m_y = y;
    m_rot = rotation;
    m_useCartesian = true;
    m_usePolar = false;
}

void COREDrive::Cartesian(double x, double y, double rotation, double throttle) {
    m_throttle = throttle;
    m_useThrottle = true;
    Cartesian(x, y, rotation);
}

void COREDrive::SetX(double x) {
    m_x = x;
}

void COREDrive::SetY(double y) {
    m_y = y;
}

double COREDrive::GetX() {
    return m_x;
}

double COREDrive::GetY() {
    return m_y;
}

void COREDrive::Polar(double magnitude, double direction, double rotation) {
    m_mag = magnitude;
    m_direction = direction;
    m_rot = rotation;
    m_usePolar = true;
    m_useCartesian = false;
}

void COREDrive::Polar(double magnitude, double direction, double rotation, double throttle) {
    m_throttle = throttle;
    m_useThrottle = true;
    Polar(magnitude, direction, rotation);
}

void COREDrive::SetMagnitude(double magnitude) {
    m_mag = magnitude;
    m_useMag = true;
}

void COREDrive::SetDirection(double direction) {
    m_direction = direction;
    m_useDirection = true;
}

double COREDrive::GetMagnitude() {
    return m_mag;
}

double COREDrive::GetDirection() {
    return m_direction;
}

void COREDrive::PreLoopTask() {
    m_direction = 0;
    m_mag = 0;
    m_useThrottle = false;
    m_rot = 0;
    m_throttle = 0;
    m_useCartesian = false;
    m_usePolar = false;
    m_useMag = false;
    m_useDirection = false;
    m_x = 0;
    m_y = 0;
}

void COREDrive::PostLoopTask() {
    if(m_useMag && m_useDirection) {
        m_x = m_mag * ToDegrees(cos(ToRadians(m_direction)));
        m_y = m_mag * ToDegrees(sin(ToRadians(m_direction)));
    }
    Update();
}
