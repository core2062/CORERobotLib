#include "COREEtherDrive.h"
#include "ctre/Phoenix.h"

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

/**
 * Sets variables "a" and "b"
 * @param a Unsure what "a" is for
 * @param b Unsure what "b" is for
 */
void COREEtherDrive::SetAB(double a, double b) {
	m_a = a;
	m_b = b;
}


void COREEtherDrive::SetQuickturn(double q) {
	m_quickTurn = q;
}


/**
 * Calculates the left and right drivetrain values using the x and y inputs from the joysticks
 * @param mag The magnitude of the joystick
 * @param rot The rotation of the joystick
 * @param deadband The deadband of the joystick
 * @return VelocityPair {left,right}; The VelocityPair value returned are left and right sides of the drivetrain
 */
VelocityPair COREEtherDrive::Calculate(double mag, double rot, double deadband) {
	if (abs(mag) < deadband) {
		mag = 0;
	}
	if (abs(rot) < deadband) {
		rot = 0;
	}
	double left, right;
	if (mag > 0) {
		if (rot >= 0) {
			left = EtherL(mag, rot, m_a, m_b);
			right = EtherR(mag, rot, m_a, m_b);
		} else {
			left = EtherR(mag, -rot, m_a, m_b);
			right = EtherL(mag, -rot, m_a, m_b);
		}
	} else if (mag < 0) {
		if (rot >= 0) {

			left = -EtherR(-mag, rot, m_a, m_b);
			right = -EtherL(-mag, rot, m_a, m_b);
		} else {
			left = -EtherL(-mag, -rot, m_a, m_b);
			right = -EtherR(-mag, -rot, m_a, m_b);
		}
	} else {
		left = (rot * m_quickTurn);
		right = (-rot * m_quickTurn);
	}

	return {left, right};
}


/**
 * Calculates the Ether Value for the left side of the drivetrain
 * @param  fwd Magnitude
 * @param  rcw Rotation
 * @param  a Unsure
 * @param  b Unsure
 * @return Value of EtherDrive Left
 */
double COREEtherDrive::EtherL(double fwd, double rcw, double a, double b) {
	return fwd + b * rcw * (1 - fwd);
}

/**
 * Calculates the Ether Value for the right side of the drivetrain
 * @param  fwd Magnitude
 * @param  rcw Rotation
 * @param  a Unsure
 * @param  b Unsure
 * @return Value of EtherDrive right
 */
double COREEtherDrive::EtherR(double fwd, double rcw, double a, double b) {
	return fwd - b * rcw + fwd * rcw * (b - a - 1);
}

/**
 * Unused
 */
void COREEtherDrive::Update() {
	if (m_y > 0) {
		if (m_rot >= 0) {
			m_left = EtherL(m_y, m_rot, m_a, m_b);
			m_right = EtherR(m_y, m_rot, m_a, m_b);
		} else {
			m_left = EtherR(m_y, -m_rot, m_a, m_b);
			m_right = EtherL(m_y, -m_rot, m_a, m_b);
		}
	} else if (m_y < 0) {
		if (m_rot >= 0) {

			m_left = -EtherR(-m_y, m_rot, m_a, m_b);
			m_right = -EtherL(-m_y, m_rot, m_a, m_b);
		} else {
			m_left = -EtherL(-m_y, -m_rot, m_a, m_b);
			m_right = -EtherR(-m_y, -m_rot, m_a, m_b);
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
