#include "TankTracker.h"

TankTracker* TankTracker::m_instance = nullptr;

TankTracker::TankTracker() {

}

TankTracker * TankTracker::GetInstance() {
    if(!m_instance) {
        m_instance = new TankTracker;
    }
    return m_instance;
}

void TankTracker::init(CANTalon * left, CANTalon * right, AHRS * gyro) {
	std::cout << "Tracker Construct" << std::endl;
	m_left = left;
	m_right = right;
	m_gyro = gyro;
    m_mainLoop = new thread(&TankTracker::loop, TankTracker::GetInstance());
    m_mainLoop->join();
	std::cout << "Tracker Contruct End" << std::endl;
}

void TankTracker::reset(double time, Position2d initial) {
    m_loopLock.lock();
	m_data = InterpolatingTreeMap(100);
	m_data.put(InterpolatingDouble(time), initial);
	m_velocity = Position2d::Delta(0,0,0);
    m_loopLock.unlock();
}

void TankTracker::start() {
    m_loopLock.lock();
	std::pair<double, double> inches = getEncoderInches();
	m_leftPrev = inches.first;
	m_rightPrev = inches.second;
    m_loopLock.unlock();
    m_mainLoop->detach();

    m_timerLock.lock();
    m_loopTimer.Reset();
    m_loopTimer.Start();
    m_timerLock.unlock();
}

void TankTracker::stop() {
    m_mainLoop->join();
}

void TankTracker::loop() {
	double time = Timer::GetFPGATimestamp();
	std::pair<double, double> inches = getEncoderInches();
	double left = inches.first;
	double right = inches.second;
	Rotation2d gyroAngle = getGyroAngle();

    m_loopLock.lock();
	Position2d odometry = generateOdometry(left - m_leftPrev,
			right - m_rightPrev, gyroAngle);
	std::pair<double, double> inPerSec = getEncoderSpeed();
    Position2d::Delta velocity = TankKinematics::forwardKinematics(inPerSec.first, inPerSec.second);
	addData(time, odometry, velocity);
	m_leftPrev = left;
	m_rightPrev = right;
    m_loopLock.unlock();

    m_loopLock.lock();
    double loopTime = m_loopTimer.Get() < m_targetLoopTime ? m_targetLoopTime - m_loopTimer.Get() : 0.0;
    if(m_loopTimer.Get() >= m_targetLoopTime * 1.2) {
        CORELog::logWarning("Tracker loop timer high at " + to_string(m_loopTimer.Get()) + " seconds!");
    }
    Wait(loopTime);
    m_loopTimer.Reset();
    m_loopTimer.Start();
    m_loopLock.unlock();
}

Position2d TankTracker::getFieldToVehicle(double time) {
    m_loopLock.lock();
    auto cache = m_data.getInterpolated(InterpolatingDouble(time));
    m_loopLock.unlock();
	return cache;
}

Position2d TankTracker::getLatestFieldToVehicle() {
    m_loopLock.lock();
    auto cache = m_data.getLatest();
    m_loopLock.unlock();
	return cache;
}

void TankTracker::addData(double time, Position2d data, Position2d::Delta vel) {
    m_loopLock.lock();
	m_data.put(InterpolatingDouble(time), data);
	m_velocity = vel;
    m_loopLock.unlock();
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
