#include "TankTracker.h"

TankTracker::TankTracker(CANTalon* left, CANTalon* right, AHRS* gyro) {
	std::cout << "Tracker Construct" << std::endl;
	m_left = left;
	m_right = right;
	m_gyro = gyro;
	std::cout << "Tracker Contruct End" << std::endl;
}

void TankTracker::reset(double time, Position2d initial) {
	m_data = InterpolatingTreeMap(100);
	m_data.put(InterpolatingDouble(time), initial);
	m_velocity = Position2d::Delta(0,0,0);
}

void TankTracker::start() {
	std::pair<double, double> inches = getEncoderInches();
	m_leftPrev = inches.first;
	m_rightPrev = inches.second;
}

void TankTracker::loop() {
	double time = Timer::GetFPGATimestamp();
	std::pair<double, double> inches = getEncoderInches();
	double left = inches.first;
	double right = inches.second;
	Rotation2d gyroAngle = getGyroAngle();
	Position2d odometry = generateOdometry(left - m_leftPrev,
			right - m_rightPrev, gyroAngle);
	std::pair<double, double> inPerSec = getEncoderSpeed();
	Position2d::Delta velocity = TankKinematics::forwardKinematics(inPerSec.first, inPerSec.second);
	addData(time, odometry, velocity);
	m_leftPrev = left;
	m_rightPrev = right;
}

Position2d TankTracker::getFieldToVehicle(double time) {
	return m_data.getInterpolated(InterpolatingDouble(time));
}

Position2d TankTracker::getLatestFieldToVehicle() {
	return m_data.getLatest();
}

void TankTracker::addData(double time, Position2d data, Position2d::Delta vel) {
	m_data.put(InterpolatingDouble(time), data);
	m_velocity = vel;
}

Position2d TankTracker::generateOdometry(double leftDelta, double rightDelta,
		Rotation2d heading) {
	Position2d last = getLatestFieldToVehicle();
	return TankKinematics::integrateForwardKinematics(last, leftDelta, rightDelta, heading);
}

std::pair<double, double> TankTracker::getEncoderInches() {
	double factor = 4.0 * PI;
	return {m_left->GetPosition() * factor, m_right->GetPosition() * factor};
}

std::pair<double, double> TankTracker::getEncoderSpeed() {
	double factor = 4.0 * PI * .0166666666;
	return {m_left->GetSpeed() * factor, m_right->GetSpeed() * factor};
}

Rotation2d TankTracker::getGyroAngle() {
	double degrees = m_gyro->GetYaw();
	return Rotation2d::fromDegrees(degrees);
}
