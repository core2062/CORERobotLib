#include "TankTracker.h"

TankTracker* TankTracker::m_instance = nullptr;

TankTracker::TankTracker():
	log({"X","Y","T"}){

}

TankTracker::~TankTracker() {
	Stop();
	m_left = nullptr;
	m_right = nullptr;
	m_gyro = nullptr;
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
	m_left->SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrame::Status_1_General_, floor(1000*m_targetLoopTime));
	m_left->SetStatusFramePeriod(ctre::phoenix::motorcontrol::Status_3_Quadrature, floor(1000*m_targetLoopTime));
	m_right->SetStatusFramePeriod(ctre::phoenix::motorcontrol::StatusFrame::Status_1_General_, floor(1000*m_targetLoopTime));
	m_right->SetStatusFramePeriod(ctre::phoenix::motorcontrol::Status_3_Quadrature, floor(1000*m_targetLoopTime));
	m_loopEnabled = false;
	std::cout << "Tracker Contruct End" << std::endl;
}

void TankTracker::Reset(double time, TankPosition2d initial) {
	m_dataLock.lock();
	m_data = TankInterpolatingTreeMap(100);
	m_data.Put(TankInterpolatingDouble(Timer::GetFPGATimestamp()), initial);
	m_data.Put(TankInterpolatingDouble(Timer::GetFPGATimestamp()+.00001), initial);
	m_velocity = TankPosition2d::TankDelta(0,0,0);
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
    m_mainLoop = new std::thread(&TankTracker::Loop, TankTracker::GetInstance());
    SetThreadPriority(*m_mainLoop, false, 3);
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
		TankRotation2d gyroAngle = GetGyroAngle();

		m_loopLock.lock();
		TankPosition2d odometry = GenerateOdometry(left - m_leftPrev, right - m_rightPrev, gyroAngle);
		m_leftPrev = left;
		m_rightPrev = right;
		m_loopLock.unlock();
		std::pair<double, double> inPerSec = GetEncoderSpeed();
		TankPosition2d::TankDelta velocity = TankKinematics::ForwardKinematics(inPerSec.first, inPerSec.second);
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

TankPosition2d TankTracker::GetFieldToVehicle(double time) {
	m_dataLock.lock();
    auto cache = m_data.GetInterpolated(TankInterpolatingDouble(time));
    m_dataLock.unlock();
	return cache;
}

TankPosition2d TankTracker::GetLatestFieldToVehicle() {
	m_dataLock.lock();
    auto cache = m_data.GetLatest();
    m_dataLock.unlock();
	return cache;
}

void TankTracker::AddData(double time, TankPosition2d data, TankPosition2d::TankDelta vel) {
	m_dataLock.lock();
	m_data.Put(TankInterpolatingDouble(time), data);
	m_velocity = vel;
	if(doLog){
		log.PutData({new COREDataPoint<double>(data.GetTranslation().GetX()),
			new COREDataPoint<double>(data.GetTranslation().GetY()),
			new COREDataPoint<double>(data.GetRotation().GetDegrees())});
	}
	m_dataLock.unlock();
}

TankPosition2d TankTracker::GenerateOdometry(double leftDelta, double rightDelta,
		TankRotation2d heading) {
	TankPosition2d last = GetLatestFieldToVehicle();
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

TankRotation2d TankTracker::GetGyroAngle() {
	double degrees = m_gyro->GetAngle();
	return TankRotation2d::FromDegrees(degrees);
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
//	m_dataLock.lock();
	log.Save(name);
//	m_dataLock.unlock();
}

void TankTracker::PostLoopTask() {
	//Position2d current = getLatestFieldToVehicle();
	auto current = GetLatestFieldToVehicle();
	SmartDashboard::PutNumber("Robot X" , current.GetTranslation().GetX());
	SmartDashboard::PutNumber("Robot Y" , current.GetTranslation().GetY());
}

void TankTracker::TeleopEndTask() {
	Stop();
}
