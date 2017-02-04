#include "COREDrive.h"

using namespace CORE;

COREDrive::COREDrive(){
    preLoopTask();
}


void COREDrive::setRotation(double rotation) {
    m_rot = rotation;
}

double COREDrive::getRotation() {
    return m_rot;
}

void COREDrive::setThottle(double throttle) {
    m_throttle = throttle;
}

double COREDrive::getThrottle() {
    return m_throttle;
}

void COREDrive::cartesian(double x, double y, double rotation) {
    m_x = x;
    m_y = y;
    m_rot = rotation;
    m_useCartesian = true;
    m_usePolar = false;
}

void COREDrive::cartesian(double x, double y, double rotation, double throttle) {
    m_throttle = throttle;
    m_useThrottle = true;
    cartesian(x, y, rotation);
}

void COREDrive::setX(double x) {
    m_x = x;
}

void COREDrive::setY(double y) {
    m_y = y;
}

double COREDrive::getX() {
    return m_x;
}

double COREDrive::getY() {
    return m_y;
}

void COREDrive::polar(double magnitude, double direction, double rotation) {
    m_mag = magnitude;
    m_direction = direction;
    m_rot = rotation;
    m_usePolar = true;
    m_useCartesian = false;
}

void COREDrive::polar(double magnitude, double direction, double rotation, double throttle) {
    m_throttle = throttle;
    m_useThrottle = true;
    polar(magnitude, direction, rotation);
}

void COREDrive::setMagnitude(double magnitude) {
    m_mag = magnitude;
    m_useMag = true;
}

void COREDrive::setDirection(double direction) {
    m_direction = direction;
    m_useDirection = true;
}

double COREDrive::getMagnitude() {
    return m_mag;
}

double COREDrive::getDirection() {
    return m_direction;
}

void COREDrive::preLoopTask() {
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

void COREDrive::postLoopTask() {
    if(m_useMag && m_useDirection) {
        m_x = m_mag * toDegrees(cos(toRadians(m_direction)));
        m_y = m_mag * toDegrees(sin(toRadians(m_direction)));
    }
    update();
}
