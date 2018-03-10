#include "SwerveTracker.h"
#include "ctre/Phoenix.h"
#include "COREDrive/CORESwerveDrive.h"

using namespace CORE;

SwerveTracker *SwerveTracker::m_instance = nullptr;

SwerveTracker::SwerveTracker() :
        log({"X", "Y", "T"}),
		m_fudgeFactor("Fudge Factor", 1) {
}

SwerveTracker::~SwerveTracker() {
    m_gyro = nullptr;
    stop();
    delete m_mainLoop;
}

SwerveTracker *SwerveTracker::GetInstance() {

    if (!m_instance) {
        m_instance = new SwerveTracker();
    }
    return m_instance;
}

void SwerveTracker::injectCORESwerve(CORESwerve * swerveDrive) {
	m_swerveDrive = swerveDrive;
}
void SwerveTracker::init(TalonSRX *steer, TalonSRX *drive, AHRS *gyro) {
    m_steer = steer;
    m_drive = drive;
    m_gyro = gyro;
    m_targetLoopTime = 1.0 / m_targetLoopHz;
    //StatusFrameRateQuadEncoder may need to be changed
    m_steer->SetStatusFramePeriod(StatusFrame::Status_1_General_,
    		m_targetLoopTime, floor(1000 * m_targetLoopTime));
    m_steer->SetStatusFramePeriod(StatusFrame::Status_1_General_,
    		m_targetLoopTime, floor(1000 * m_targetLoopTime));
    m_drive->SetStatusFramePeriod(StatusFrame::Status_1_General_,
    		m_targetLoopTime, floor(1000 * m_targetLoopTime));
    m_drive->SetStatusFramePeriod(StatusFrame::Status_1_General_,
    		m_targetLoopTime, floor(1000 * m_targetLoopTime));
    m_loopEnabled = false;
}

void SwerveTracker::reset(double time, Position2d initial) {
    m_dataLock.lock();
    m_data = InterpolatingTreeMap(100);
    m_data.put(InterpolatingDouble(Timer::GetFPGATimestamp()), initial);
    m_data.put(InterpolatingDouble(Timer::GetFPGATimestamp() + .00001), initial);
    m_velocity = Position2d::Delta(0, 0, 0);
    m_dataLock.unlock();
}

void SwerveTracker::start() {
    CORELog::logInfo("Starting Swerve Tracker!");
    m_loopLock.lock();
    std::pair<double, double> inches = getEncoderInches();
    m_leftPrev = inches.first;
    m_rightPrev = inches.second;
    m_loopLock.unlock();

    m_timerLock.lock();
    m_loopTimer.Reset();
    m_loopTimer.Start();
    m_timerLock.unlock();

    m_loopEnabled = true;
    m_mainLoop = new thread(&SwerveTracker::loop, SwerveTracker::GetInstance());
    CORELog::logInfo("Started Swerve tracker!");
}

void SwerveTracker::stop() {
    if (m_loopEnabled && m_mainLoop) {
        m_loopEnabled = false;
        m_mainLoop->join();
    }
}

void SwerveTracker::loop() {
    while (m_loopEnabled) {
        double time = CORETimer::getTime();
        std::pair<double, double> inches = getEncoderInches();
        COREVector gyroAngle = getGyroAngle();

        m_loopLock.lock();
        m_loopLock.unlock();
        std::pair<double, double> inPerSec = getEncoderSpeed();

        m_timerLock.lock();
        double loopTime = m_loopTimer.Get() < m_targetLoopTime ? m_targetLoopTime - m_loopTimer.Get() : 0.0;
        if (m_loopTimer.Get() >= m_targetLoopTime * 1.2) {
            CORELog::logWarning("Tracker loop timer high at " + to_string(m_loopTimer.Get()) + " seconds!");
        }
        Wait(loopTime);
        m_loopTimer.Reset();
        m_loopTimer.Start();
        m_timerLock.unlock();
    }
}

Position2d SwerveTracker::getFieldToVehicle(double time) {
    m_dataLock.lock();
    auto cache = m_data.getInterpolated(InterpolatingDouble(time));
    m_dataLock.unlock();
    return cache;
}

Position2d SwerveTracker::getLatestFieldToVehicle() {
    m_dataLock.lock();
    auto cache = m_data.getLatest();
    m_dataLock.unlock();
    return cache;
}

void SwerveTracker::addData(double time, Position2d data, Position2d::Delta vel) {
    m_dataLock.lock();
    m_data.put(InterpolatingDouble(time), data);
    m_velocity = vel;
    if (doLog) {
        log.putData({new COREDataPoint<double>(data.getTranslation().GetX()),
                     new COREDataPoint<double>(data.getTranslation().GetY()),
                     new COREDataPoint<double>(data.getRotation().GetDegrees())});
    }
    m_dataLock.unlock();
}

COREVector SwerveTracker::generateOdometry() {
    return m_swerveDrive->forwardKinematics(0);
}

std::pair<double, double> SwerveTracker::getEncoderInches() {
    double factor = 4.0 * PI;
    return {m_steer->GetSelectedSensorPosition(0) * factor, m_drive->GetSelectedSensorPosition(0) * factor};
}

//TODO change the inputs of the getSelectedSensorPosition and getSelectedSensorVelocity

std::pair<double, double> SwerveTracker::getEncoderSpeed() {
    double factor = 4.0 * PI * .0166666666;
    return {m_steer->GetSelectedSensorVelocity(0) * factor, m_drive->GetSelectedSensorVelocity(0) * factor};
}

COREVector SwerveTracker::getGyroAngle() {
    double degrees = m_gyro->GetAngle();
    return COREVector::FromDegrees(degrees);
}

void SwerveTracker::autonInitTask() {
    doLog = true;
    start();
}

void SwerveTracker::autonEndTask() {
    stop();
}

void SwerveTracker::teleopInitTask() {
    stop();
    doLog = false;
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    std::string name = "AutonLog";
    name += to_string(now->tm_mon) + "-" + to_string(now->tm_mday) + "--" + to_string(now->tm_hour)
            + "-" + to_string(now->tm_min) + ".csv";
    log.save(name);
}

void SwerveTracker::postLoopTask() {

}

void SwerveTracker::teleopEndTask() {
    stop();
}
