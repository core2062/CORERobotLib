#include "COREEtherDrive.h"

using namespace CORE;

double COREEtherDrive::m_a = 0.0;
double COREEtherDrive::m_b = 0.0;
double COREEtherDrive::m_quickTurn = 0.0;

COREEtherDrive::COREEtherDrive(TalonSRX * leftMotor1, TalonSRX * leftMotor2,
		TalonSRX * rightMotor1, TalonSRX * rightMotor2, double a, double b,
		double quickTurn) :
		m_leftMotor1(leftMotor1), m_leftMotor2(leftMotor2), m_rightMotor1(
				rightMotor1), m_rightMotor2(rightMotor2) {
	m_a = a;
	m_b = b;
	m_quickTurn = quickTurn;

}

void COREEtherDrive::setAB(double a, double b) {
	m_a = a;
	m_b = b;
}

void COREEtherDrive::setQuickturn(double q) {
	m_quickTurn = q;
}

VelocityPair COREEtherDrive::calculate(double mag, double rot,
		double deadband) {
	if (abs(mag) < deadband) {
		mag = 0;
	}
	if (abs(rot) < deadband) {
		rot = 0;
	}
	double left, right;
	if (mag > 0) {
		if (rot >= 0) {
			left = etherL(mag, rot, m_a, m_b);
			right = etherR(mag, rot, m_a, m_b);
		} else {
			left = etherR(mag, -rot, m_a, m_b);
			right = etherL(mag, -rot, m_a, m_b);
		}
	} else if (mag < 0) {
		if (rot >= 0) {

			left = -etherR(-mag, rot, m_a, m_b);
			right = -etherL(-mag, rot, m_a, m_b);
		} else {
			left = -etherL(-mag, -rot, m_a, m_b);
			right = -etherR(-mag, -rot, m_a, m_b);
		}
	} else {
		left = (rot * m_quickTurn);
		right = (-rot * m_quickTurn);
	}

	return {left, right};
}

double COREEtherDrive::etherL(double fwd, double rcw, double a, double b) {
	return fwd + b * rcw * (1 - fwd);
}

double COREEtherDrive::etherR(double fwd, double rcw, double a, double b) {
	return fwd - b * rcw + fwd * rcw * (b - a - 1);
}

void COREEtherDrive::update() {
	if (m_y > 0) {
		if (m_rot >= 0) {
			m_left = etherL(m_y, m_rot, m_a, m_b);
			m_right = etherR(m_y, m_rot, m_a, m_b);
		} else {
			m_left = etherR(m_y, -m_rot, m_a, m_b);
			m_right = etherL(m_y, -m_rot, m_a, m_b);
		}
	} else if (m_y < 0) {
		if (m_rot >= 0) {

			m_left = -etherR(-m_y, m_rot, m_a, m_b);
			m_right = -etherL(-m_y, m_rot, m_a, m_b);
		} else {
			m_left = -etherL(-m_y, -m_rot, m_a, m_b);
			m_right = -etherR(-m_y, -m_rot, m_a, m_b);
		}
	} else {
		m_left = (m_rot * m_quickTurn);
		m_right = (-m_rot * m_quickTurn);
	}
	m_leftMotor1->Set(ControlMode::Velocity, m_left);
	m_rightMotor1->Set(ControlMode::Velocity, m_right);
	m_leftMotor2->Set(ControlMode::Velocity, m_left);
	m_rightMotor2->Set(ControlMode::Velocity, m_right);
}
