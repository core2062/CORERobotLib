#include "COREEtherDrive.h"

using namespace CORE;

COREEtherDrive::COREEtherDrive(COREMotor * leftMotor1, COREMotor * leftMotor2, COREMotor * rightMotor1,
                               COREMotor * rightMotor2, double a, double b, double quickTurn) :
        m_leftMotor1(leftMotor1), m_leftMotor2(leftMotor2), m_rightMotor1(rightMotor1), m_rightMotor2(rightMotor2),
        m_a(a), m_b(b), m_quickTurn(quickTurn) {

}

double COREEtherDrive::etherL(double fwd, double rcw, double a, double b) {
    return fwd + b * rcw * (1 - fwd);
}

double COREEtherDrive::etherR(double fwd, double rcw, double a, double b) {
    return fwd - b * rcw + fwd * rcw * (b - a - 1);
}

void COREEtherDrive::update() {
    if (m_y > 0){
        if (m_rot >= 0){
            m_left = etherL(m_y, m_rot, m_a, m_b);
            m_right = etherR(m_y, m_rot, m_a, m_b);
        } else{
            m_left = etherR(m_y, -m_rot, m_a, m_b);
            m_right = etherL(m_y, -m_rot, m_a, m_b);
        }
    } else if (m_y < 0) {
        if (m_rot>=0){

            m_left = -etherR(-m_y, m_rot, m_a, m_b);
            m_right = -etherL(-m_y, m_rot, m_a, m_b);
        } else{
            m_left = -etherL(-m_y, -m_rot, m_a, m_b);
            m_right = -etherR(-m_y, -m_rot, m_a, m_b);
        }
    } else {
        m_left = (m_rot * m_quickTurn);
        m_right = (-m_rot * m_quickTurn);
    }
    m_leftMotor1->Set(m_left);
    m_rightMotor1->Set(m_right);
    m_leftMotor2->Set(m_left);
    m_rightMotor2->Set(m_right);
}
