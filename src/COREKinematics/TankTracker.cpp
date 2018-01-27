#include "TankTracker.h"

using namespace CORE;

TankTracker *TankTracker::m_instance = nullptr;

TankTracker::TankTracker() :
        log({"X", "Y", "T"}) {

}

TankTracker::~TankTracker() {
    m_left = nullptr;
    m_right = nullptr;
    m_gyro = nullptr;
    stop();
    delete m_mainLoop;
}

TankTracker *TankTracker::GetInstance() {
    if (!m_instance) {
        m_instance = new TankTracker;
    }
    return m_instance;
}

void TankTracker::init(TalonSRX *left, TalonSRX *right, AHRS *gyro) {
    m_left = left;
    m_right = right;
    m_gyro = gyro;
    m_targetLoopTime = 1.0 / m_targetLoopHz;
    m_left->SetStatusFrameRateMs(TalonSRX::StatusFrameRateGeneral, floor(1000 * m_targetLoopTime));
    m_left->SetStatusFrameRateMs(TalonSRX::StatusFrameRateQuadEncoder, floor(1000 * m_targetLoopTime));
    m_right->SetStatusFrameRateMs(TalonSRX::StatusFrameRateGeneral, floor(1000 * m_targetLoopTime));
    m_right->SetStatusFrameRateMs(TalonSRX::StatusFrameRateQuadEncoder, floor(1000 * m_targetLoopTime));
    m_loopEnabled = false;
}

void TankTracker::reset(double time, Position2d initial) {
    m_dataLock.lock();
    m_data = InterpolatingTreeMap(100);
    m_data.put(InterpolatingDouble(Timer::GetFPGATimestamp()), initial);
    m_data.put(InterpolatingDouble(Timer::GetFPGATimestamp() + .00001), initial);
    m_velocity = Position2d::Delta(0, 0, 0);
    m_dataLock.unlock();
}

void TankTracker::start() {
    CORELog::logInfo("Starting tank tracker!");
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
    m_mainLoop = new thread(&TankTracker::loop, TankTracker::GetInstance());
    //SetThreadPriority(*m_mainLoop, false, 3);
    CORELog::logInfo("Started tank tracker!");
}

void TankTracker::stop() {
    if (m_loopEnabled && m_mainLoop) {
        m_loopEnabled = false;
        m_mainLoop->join();
    }
}

void TankTracker::loop() {
    while (m_loopEnabled) {
        double time = CORETimer::getTime();//Timer::GetFPGATimestamp();
        std::pair<double, double> inches = getEncoderInches();
        double left = inches.first;
        double right = inches.second;
        COREVector gyroAngle = getGyroAngle();

        m_loopLock.lock();
        Position2d odometry = generateOdometry(left - m_leftPrev, right - m_rightPrev, gyroAngle);
        m_leftPrev = left;
        m_rightPrev = right;
        m_loopLock.unlock();
        std::pair<double, double> inPerSec = getEncoderSpeed();
        Position2d::Delta velocity = TankKinematics::forwardKinematics(inPerSec.first, inPerSec.second);
        addData(time, odometry, velocity);

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

Position2d TankTracker::getFieldToVehicle(double time) {
    m_dataLock.lock();
    auto cache = m_data.getInterpolated(InterpolatingDouble(time));
    m_dataLock.unlock();
    return cache;
}

Position2d TankTracker::getLatestFieldToVehicle() {
    m_dataLock.lock();
    auto cache = m_data.getLatest();
    m_dataLock.unlock();
    return cache;
}

void TankTracker::addData(double time, Position2d data, Position2d::Delta vel) {
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

Position2d TankTracker::generateOdometry(double leftDelta, double rightDelta,
                                         COREVector heading) {
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

COREVector TankTracker::getGyroAngle() {
    double degrees = m_gyro->GetAngle();
    return COREVector::FromDegrees(degrees);
}

void TankTracker::autonInitTask() {
    doLog = true;
    start();
}

void TankTracker::autonEndTask() {
    stop();
}

void TankTracker::teleopInitTask() {
    stop();
    doLog = false;
    time_t currentTime = time(0);
    struct tm *now = localtime(&currentTime);
    std::string name = "AutonLog";
    name += to_string(now->tm_mon) + "-" + to_string(now->tm_mday) + "--" + to_string(now->tm_hour)
            + "-" + to_string(now->tm_min) + ".csv";
    log.save(name);
}

void TankTracker::postLoopTask() {

}

void TankTracker::teleopEndTask() {
    stop();
}
