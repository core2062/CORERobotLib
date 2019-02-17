#include "SwerveTracker.h"

SwerveTracker* SwerveTracker::m_instance = nullptr;

SwerveTracker::SwerveTracker():
	log({"X","Y","T"}){

}

SwerveTracker::~SwerveTracker() {
	m_left = nullptr;
	m_right = nullptr;
	m_gyro = nullptr;
	Stop();
	delete m_mainLoop;
}

SwerveTracker * SwerveTracker::GetInstance() {
    if(!m_instance) {
        m_instance = new SwerveTracker;
    }
    return m_instance;
}

void SwerveTracker::Init(TalonSRX * left, TalonSRX * right, AHRS * gyro) {
	std::cout << "Tracker Construct" << std::endl;
	m_left = left;
	m_right = right;
	m_gyro = gyro;
	m_targetLoopTime = 1.0/m_targetLoopHz;
    m_left->SetStatusFramePeriod(StatusFrameEnhanced::Status_1_General, floor(1000*m_targetLoopTime), 0);
    m_left->SetStatusFramePeriod(StatusFrameEnhanced::Status_3_Quadrature, floor(1000*m_targetLoopTime), 0);
	m_right->SetStatusFramePeriod(StatusFrameEnhanced::Status_1_General, floor(1000*m_targetLoopTime), 0);
	m_right->SetStatusFramePeriod(StatusFrameEnhanced::Status_3_Quadrature, floor(1000*m_targetLoopTime), 0);
	m_loopEnabled = false;
	std::cout << "Tracker Contruct End" << std::endl;
}

void SwerveTracker::Reset(double time, SwervePosition2d initial) {
	m_dataLock.lock();
	m_data = SwerveInterpolatingTreeMap(100);
	m_data.Put(SwerveInterpolatingDouble(Timer::GetFPGATimestamp()), initial);
	m_data.Put(SwerveInterpolatingDouble(Timer::GetFPGATimestamp()+.00001), initial);
	m_velocity = SwervePosition2d::SwerveDelta(0,0,0);
	m_dataLock.unlock();
}

void SwerveTracker::Start() {
	cout << "Starting tank tracker!" << endl;
    m_loopLock.lock();
	std::pair<double, double> inches = GetEncoderInches();
	m_leftPrev = inches.first;
	m_rightPrev = inches.second;
    m_loopLock.unlock();

    m_timerLock.lock();
    m_loopTimer.Reset();
    m_loopTimer.Start();
    m_timerLock.unlock();

    m_loopEnabled = true;
    m_mainLoop = new thread(&SwerveTracker::Loop, SwerveTracker::GetInstance());
    frc::SetCurrentThreadPriority(false, 3);
	cout << "Started tank tracker!" << endl;
}

void SwerveTracker::Stop() {
	if(m_loopEnabled && m_mainLoop){
		m_loopEnabled = false;
		m_mainLoop->join();
	}
}

void SwerveTracker::Loop() {
	while(m_loopEnabled) {
		double time = Timer::GetFPGATimestamp();
		std::pair<double, double> inches = GetEncoderInches();
		double left = inches.first;
		double right = inches.second;
		SwerveRotation2d gyroAngle = GetGyroAngle();

		m_loopLock.lock();
		SwervePosition2d odometry = GenerateOdometry(left - m_leftPrev, right - m_rightPrev, gyroAngle);
		m_leftPrev = left;
		m_rightPrev = right;
		m_loopLock.unlock();
		std::pair<double, double> inPerSec = GetEncoderSpeed();
		SwervePosition2d::SwerveDelta velocity = SwerveKinematics::ForwardKinematics(inPerSec.first, inPerSec.second);
		AddData(time, odometry, velocity);

		m_timerLock.lock();
		double loopTime = m_loopTimer.Get() < m_targetLoopTime ? m_targetLoopTime - m_loopTimer.Get() : 0.0;
		if(m_loopTimer.Get() >= m_targetLoopTime * 1.2) {
			CORELog::LogWarning("Tracker loop timer high at " + to_string(m_loopTimer.Get()) + " seconds!");
		}
		Wait(loopTime);
		m_loopTimer.Reset();
		m_loopTimer.Start();
		m_timerLock.unlock();
	}
}

SwervePosition2d SwerveTracker::GetFieldToVehicle(double time) {
	m_dataLock.lock();
    auto cache = m_data.GetInterpolated(SwerveInterpolatingDouble(time));
    m_dataLock.unlock();
	return cache;
}

SwervePosition2d SwerveTracker::GetLatestFieldToVehicle() {
	m_dataLock.lock();
    auto cache = m_data.GetLatest();
    m_dataLock.unlock();
	return cache;
}

void SwerveTracker::AddData(double time, SwervePosition2d data, SwervePosition2d::SwerveDelta vel) {
	m_dataLock.lock();
	m_data.Put(SwerveInterpolatingDouble(time), data);
	m_velocity = vel;
	if(doLog){
		log.PutData({new COREDataPoint<double>(data.GetTranslation().GetX()),
			new COREDataPoint<double>(data.GetTranslation().GetY()),
			new COREDataPoint<double>(data.GetRotation().GetDegrees())});
	}
	m_dataLock.unlock();
}

SwervePosition2d SwerveTracker::GenerateOdometry(double leftDelta, double rightDelta,
		SwerveRotation2d heading) {
	SwervePosition2d last = GetLatestFieldToVehicle();
	return SwerveKinematics::IntegrateForwardKinematics(last, leftDelta, rightDelta, heading);
}

std::pair<double, double> SwerveTracker::GetEncoderInches() {
	double factor = 4.0 * PI;
	return {m_left->GetSelectedSensorPosition() * factor, m_right->GetSelectedSensorPosition() * factor};
}

std::pair<double, double> SwerveTracker::GetEncoderSpeed() {
	double factor = 4.0 * PI * .0166666666;
	return {m_left->GetSelectedSensorVelocity() * factor, m_right->GetSelectedSensorVelocity() * factor};
}

SwerveRotation2d SwerveTracker::GetGyroAngle() {
	double degrees = m_gyro->GetAngle();
	return SwerveRotation2d::FromDegrees(degrees);
}

void SwerveTracker::AutonInitTask() {
	doLog = true;
	Start();
}

void SwerveTracker::AutonEndTask() {
	Stop();
}

void SwerveTracker::TeleopInitTask() {
	Stop();
	doLog = false;
    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);
    std::string name = "AutonLog";
    name += to_string(now->tm_mon) + "-" + to_string(now->tm_mday) + "--" + to_string(now->tm_hour)
                  + "-" + to_string(now->tm_min) + ".csv";
	log.Save(name);
}

void SwerveTracker::PostLoopTask() {
	auto current = GetLatestFieldToVehicle();
	SmartDashboard::PutNumber("Robot X" , current.GetTranslation().GetX());
	SmartDashboard::PutNumber("Robot Y" , current.GetTranslation().GetY());
}

void SwerveTracker::TeleopEndTask() {
	Stop();
}