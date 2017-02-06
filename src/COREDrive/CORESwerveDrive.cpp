#include "CORESwerveDrive.h"

using namespace CORE;

CORESwerve::CORESwerve(double trackWidth, double wheelBase, CORESwerve::SwerveModule& leftFrontModule,
                       CORESwerve::SwerveModule& leftBackModule, CORESwerve::SwerveModule& rightBackModule,
                       CORESwerve::SwerveModule& rightFrontModule) :
        m_leftFrontModule(&leftFrontModule), m_leftBackModule(&leftBackModule),
        m_rightBackModule(&rightBackModule), m_rightFrontModule(&rightFrontModule) {
    m_modules.push_back(m_leftFrontModule);
    m_modules.push_back(m_leftBackModule);
    m_modules.push_back(m_rightBackModule);
    m_modules.push_back(m_rightFrontModule);
    m_trackwidth = trackWidth;
    m_wheelbase = wheelBase;
    double x = m_trackwidth*.5;
    double y = m_wheelbase*.5;
    m_leftFrontModule->position = {-x, y};
    m_leftBackModule->position = {-x, -y};
    m_rightBackModule->position = {x, -y};
    m_rightFrontModule->position = {x, y};
}

void CORESwerve::setRotation(double rotation) {
        m_rot = rotation;
}

double CORESwerve::getRotation() {
    return m_rot;
}

void CORESwerve::setThrottle(double throttle) {
    m_throttle = throttle;
}

double CORESwerve::getThrottle() {
    return m_throttle;
}

void CORESwerve::cartesian(double x, double y, double rotation) {
    m_x = x;
    m_y = y;
    m_rot = rotation;
}

void CORESwerve::cartesian(double x, double y, double rotation, double throttle) {
    m_x = x;
    m_y = y;
    m_rot = rotation;
    m_throttle = throttle;
}

void CORESwerve::setX(double x) {
    m_x = x;
}

void CORESwerve::setY(double y) {
    m_y = y;
}

double CORESwerve::getX() {
    return m_x;
}

double CORESwerve::getY() {
    return m_y;
}

void CORESwerve::polar(double magnitude, double direction, double rotation) {
    m_x = magnitude * toDegrees(cos(toRadians(direction)));
    m_y = magnitude * toDegrees(sin(toRadians(direction)));
    m_rot = rotation;
}

void CORESwerve::polar(double magnitude, double direction, double rotation, double throttle) {
    m_x = magnitude * toDegrees(cos(toRadians(direction)));
    m_y = magnitude * toDegrees(sin(toRadians(direction)));
    m_rot = rotation;
    m_throttle = throttle;
}

void CORESwerve::setMagnitude(double magnitude) {

}

void CORESwerve::setDirection(double direction) {

}

double CORESwerve::getMagnitude() {
    return 0;
}

double CORESwerve::getDirection() {
    return 0;
}

void CORESwerve::update() {
    double max = 0.0;
    for (auto i : m_modules) {
        if(fabs(m_x+m_y+m_rot)>.2){
            i->m_setDirection = 1;
            double a,b;
            a = m_x - m_rot * i->position.unit().y;
            b = m_y - m_rot * i->position.unit().x;
            i->m_setMagnitude = sqrt(pow(a, 2) + pow(b, 2));
            double setAngle = arctan(a, b);
            /*if (abs(setAngle - i->getCurrentAngle()) > 180){
                setAngle = i->clamp(setAngle - 180);
                i->m_setDirection = -1;
            }*/
            i->m_setAngle = setAngle;
            i->m_setMagnitude = i->m_setMagnitude * m_throttle;
            if(fabs(i->m_setMagnitude) > max)
                max = fabs(i->m_setMagnitude);
        }
        else {
        	i->m_setMagnitude = 0;
        }
        //i->update();
    }

    if(max>1.0){
        for (auto i : m_modules){
            i->m_setMagnitude = i->m_setMagnitude / max;
        }
    }
    for (auto i : m_modules){
        i->update();
    }
}
