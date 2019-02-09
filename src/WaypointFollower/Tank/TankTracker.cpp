#include "TankTracker.h"

TankTracker* TankTracker::m_instance = nullptr;

TankTracker::TankTracker():
	log({"X","Y","T"}){

}

TankTracker::~TankTracker() {
	m_left = nullptr;
	m_right = nullptr;
	m_gyro = nullptr;
	Stop();
	delete m_mainLoop;
}

TankTracker * TankTracker::GetInstance() {
    if(!m_instance) {
        m_instance = new TankTracker;
    }
    return m_instance;
}

void TankTracker::Init(TalonSRX * left, TalonSRX * right, AHRS * gyro) {
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

void TankTracker::Reset(double time, Position2d initial) {
	m_dataLock.lock();
	m_data = InterpolatingTreeMap(100);
	m_data.Put(InterpolatingDouble(Timer::GetFPGATimestamp()), initial);
	m_data.Put(InterpolatingDouble(Timer::GetFPGATimestamp()+.00001), initial);
	m_velocity = Position2d::Delta(0,0,0);
	m_dataLock.unlock();
}

void TankTracker::Start() {
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
    m_mainLoop = new thread(&TankTracker::Loop, TankTracker::GetInstance());
    frc::SetCurrentThreadPriority(false, 3);
	cout << "Started tank tracker!" << endl;
}

void TankTracker::Stop() {
	if(m_loopEnabled && m_mainLoop){
		m_loopEnabled = false;
		m_mainLoop->join();
	}
}

void TankTracker::Loop() {
	while(m_loopEnabled) {
		double time = Timer::GetFPGATimestamp();
		std::pair<double, double> inches = GetEncoderInches();
		double left = inches.first;
		double right = inches.second;
		Rotation2d gyroAngle = GetGyroAngle();

		m_loopLock.lock();
		Position2d odometry = GenerateOdometry(left - m_leftPrev, right - m_rightPrev, gyroAngle);
		m_leftPrev = left;
		m_rightPrev = right;
		m_loopLock.unlock();
		std::pair<double, double> inPerSec = GetEncoderSpeed();
		Position2d::Delta velocity = TankKinematics::ForwardKinematics(inPerSec.first, inPerSec.second);
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

Position2d TankTracker::GetFieldToVehicle(double time) {
	m_dataLock.lock();
    auto cache = m_data.GetInterpolated(InterpolatingDouble(time));
    m_dataLock.unlock();
	return cache;
}

Position2d TankTracker::GetLatestFieldToVehicle() {
	m_dataLock.lock();
    auto cache = m_data.GetLatest();
    m_dataLock.unlock();
	return cache;
}

void TankTracker::AddData(double time, Position2d data, Position2d::Delta vel) {
	m_dataLock.lock();
	m_data.Put(InterpolatingDouble(time), data);
	m_velocity = vel;
	if(doLog){
		log.PutData({new COREDataPoint<double>(data.GetTranslation().GetX()),
			new COREDataPoint<double>(data.GetTranslation().GetY()),
			new COREDataPoint<double>(data.GetRotation().GetDegrees())});
	}
	m_dataLock.unlock();
}

Position2d TankTracker::GenerateOdometry(double leftDelta, double rightDelta,
		Rotation2d heading) {
	Position2d last = GetLatestFieldToVehicle();
	return TankKinematics::IntegrateForwardKinematics(last, leftDelta, rightDelta, heading);
}

std::pair<double, double> TankTracker::GetEncoderInches() {
	double factor = 4.0 * PI;
	return {m_left->GetSelectedSensorPosition() * factor, m_right->GetSelectedSensorPosition() * factor};
}

std::pair<double, double> TankTracker::GetEncoderSpeed() {
	double factor = 4.0 * PI * .0166666666;
	return {m_left->GetSelectedSensorVelocity() * factor, m_right->GetSelectedSensorVelocity() * factor};
}

Rotation2d TankTracker::GetGyroAngle() {
	double degrees = m_gyro->GetAngle();
	return Rotation2d::FromDegrees(degrees);
}

void TankTracker::AutonInitTask() {
	doLog = true;
	Start();
}

void TankTracker::AutonEndTask() {
	Stop();
}

void TankTracker::TeleopInitTask() {
	Stop();
	doLog = false;
    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);
    std::string name = "AutonLog";
    name += to_string(now->tm_mon) + "-" + to_string(now->tm_mday) + "--" + to_string(now->tm_hour)
                  + "-" + to_string(now->tm_min) + ".csv";
	log.Save(name);
}

void TankTracker::PostLoopTask() {
	auto current = GetLatestFieldToVehicle();
	SmartDashboard::PutNumber("Robot X" , current.GetTranslation().GetX());
	SmartDashboard::PutNumber("Robot Y" , current.GetTranslation().GetY());
}

void TankTracker::TeleopEndTask() {
	Stop();
}