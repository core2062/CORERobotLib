#include "DriveSubsystem.h"
//#include "Robot.h"

DriveSubsystem::DriveSubsystem() : m_leftFrontSteer(LFS_DRIVE_MOTOR_PORT),
                                   m_rightFrontSteer(RFS_DRIVE_MOTOR_PORT),
                                   m_leftBackSteer(LBS_DRIVE_MOTOR_PORT),
                                   m_rightBackSteer(RBS_DRIVE_MOTOR_PORT),
                                   m_rightFrontDrive(RFD_DRIVE_MOTOR_PORT),
                                   m_leftFrontDrive(LFD_DRIVE_MOTOR_PORT),
                                   m_rightBackDrive(RBD_DRIVE_MOTOR_PORT),
                                   m_leftBackDrive(LBD_DRIVE_MOTOR_PORT),
                                   m_leftBackModule(new CORESwerve::SwerveModule(&m_leftBackDrive, &m_leftBackSteer)),
                                   m_rightBackModule(new CORESwerve::SwerveModule(&m_rightBackDrive, &m_rightBackSteer)),
                                   m_rightFrontModule(new CORESwerve::SwerveModule(&m_rightFrontDrive, &m_rightBackSteer)),
                                   m_leftFrontModule(new CORESwerve::SwerveModule(&m_leftFrontDrive, &m_leftFrontSteer)),
                                   m_swerveDrive(1, 1/*27.7, 22.3*/, m_leftFrontModule, m_leftBackModule,
                                                 m_rightBackModule, m_rightFrontModule),
                                   m_gyro(m_gyro) {

}

void DriveSubsystem::robotInit() {
    try {
        m_gyro = make_shared<AHRS>(AHRS::SerialPort::Port::kUSB);
        CORELog::logInfo("NavX Initialized!");
    } catch(std::exception & ex) {
        CORELog::logWarning("Couldn't find NavX!");
    }
}

void DriveSubsystem::teleopInit() {

}

void DriveSubsystem::teleop() {

}
