#include <Joystick.h>
#include <SampleRobot.h>
#include <Talon.h>
#include <AHRS.h>
#include <Timer.h>
#include "COREDrive/CORESwerveDrive.h"

/****************************************************CAN TALONS********************************************************/
#define RFD_DRIVE_MOTOR_PORT 17
#define RFS_DRIVE_MOTOR_PORT 61
#define LFD_DRIVE_MOTOR_PORT 60
#define LFS_DRIVE_MOTOR_PORT 59
#define RBD_DRIVE_MOTOR_PORT 10
#define RBS_DRIVE_MOTOR_PORT 11
#define LBD_DRIVE_MOTOR_PORT 13
#define LBS_DRIVE_MOTOR_PORT 12

#define LEFT_CLIMB_MOTOR_PORT 14
#define RIGHT_CLIMB_MOTOR_PORT 15
//#define LIFT_MOTOR_PORT 16

/****************************************************PWM MOTORS********************************************************/
#define LIFT_MOTOR_PORT 0
#define INTAKE_MOTOR_PORT 1
#define LEFT_DUMP_FLAP_SERVO_CHANNEL 2
#define RIGHT_DUMP_FLAP_SERVO_CHANNEL 3

/****************************************************ANALOG SENSORS****************************************************/
#define LIFT_ENCODER_A_PORT 0
#define LIFT_ENCODER_B_PORT 1
#define LEFT_GEAR_ULTRA_PORT 2
#define RIGHT_GEAR_ULTRA_PORT 3

/****************************************************DIGITAL SENSORS***************************************************/
#define CLIMB_LIMIT_SWITCH_PORT 0
#define GEAR_MANIPULATOR_LIMIT_SWITCH_PORT 1
#define LIFT_BOTTOM_LIMIT_SWITCH_PORT 2
#define LIFT_TOP_LIMIT_SWITCH_PORT 3

/****************************************************SOLENOIDS*********************************************************/
#define LEFT_DRIVE_SHIFTER_HIGH_GEAR_PORT 0
#define LEFT_DRIVE_SHIFTER_LOW_GEAR_PORT 1
#define LEFT_DRIVE_SHIFTER_PCM 1
#define RIGHT_DRIVE_SHIFTER_HIGH_GEAR_PORT 2
#define RIGHT_DRIVE_SHIFTER_LOW_GEAR_PORT 3
#define RIGHT_DRIVE_SHIFTER_PCM 1
#define LEFT_GEAR_FLAP_SOLENOID_OUT_PORT 0
#define LEFT_GEAR_FLAP_SOLENOID_IN_PORT 1
#define LEFT_GEAR_FLAP_SOLENOID_PCM 2
#define RIGHT_GEAR_FLAP_SOLENOID_OUT_PORT 2
#define RIGHT_GEAR_FLAP_SOLENOID_IN_PORT 3
#define RIGHT_GEAR_FLAP_SOLENOID_PCM 2

#define GEAR_PICKUP_LEFT_OUT_PORT 4
#define GEAR_PICKUP_LEFT_IN_PORT 5
#define GEAR_PICKUP_LEFT_PCM 2
#define GEAR_PICKUP_RIGHT_OUT_PORT 4
#define GEAR_PICKUP_RIGHT_IN_PORT 5
#define GEAR_PICKUP_RIGHT_PCM 1
#define GEAR_PICKUP_FORWARD_PCM 1
#define GEAR_PICKUP_FORWARD_ON_PORT 4
#define GEAR_PICKUP_FORWARD_OFF_PORT 5
#define GEAR_PICKUP_REVERSE_PCM 2
#define GEAR_PICKUP_REVERSE_ON_PORT 4
#define GEAR_PICKUP_REVERSE_OFF_PORT 5
#define GEAR_INTAKE_PORT 16

using namespace frc;
using namespace CORE;

class Robot: public SampleRobot {
public:
	Robot() :
		m_leftFrontSteer(LFS_DRIVE_MOTOR_PORT),
		m_rightFrontSteer(RFS_DRIVE_MOTOR_PORT),
		m_leftBackSteer(LBS_DRIVE_MOTOR_PORT),
		m_rightBackSteer(RBS_DRIVE_MOTOR_PORT),
		m_rightFrontDrive(RFD_DRIVE_MOTOR_PORT),
		m_leftFrontDrive(LFD_DRIVE_MOTOR_PORT),
		m_rightBackDrive(RBD_DRIVE_MOTOR_PORT),
		m_leftBackDrive(LBD_DRIVE_MOTOR_PORT),
		m_leftBackModule(new CORESwerve::SwerveModule(&m_leftBackDrive, &m_leftBackSteer)),
		m_rightBackModule(new CORESwerve::SwerveModule(&m_rightBackDrive, &m_rightBackSteer)),
		m_rightFrontModule(new CORESwerve::SwerveModule(&m_rightFrontDrive, &m_rightFrontSteer)),
		m_leftFrontModule(new CORESwerve::SwerveModule(&m_leftFrontDrive, &m_leftFrontSteer)),
		m_swerveDrive(27.7, 22.3, m_leftFrontModule, m_leftBackModule, m_rightBackModule, m_rightFrontModule) {
			try {
				m_gyro = make_shared<AHRS>(SerialPort::Port::kUSB, AHRS::SerialDataType::kProcessedData, 80);
				CORELog::logInfo("NavX Initialized!");
			} catch(std::exception & ex) {
				CORELog::logWarning("Couldn't find NavX!");
			}
			SmartDashboard::PutNumber("Steer kP Value", 0.01);
			SmartDashboard::PutNumber("Steer kI Value", 0);
			SmartDashboard::PutNumber("Steer kD Value", 0);
			SmartDashboard::PutBoolean("Zero All Modules", false);
			SmartDashboard::PutBoolean("Field Centric", false);
			m_leftBackSteer.SetInverted(true);
			m_leftFrontSteer.SetInverted(true);
			m_rightBackSteer.SetInverted(true);
			m_rightFrontSteer.SetInverted(true);
			SmartDashboard::PutNumber("Front Left Steer Offset", 289.4);
			SmartDashboard::PutNumber("Front Right Steer Offset", 252.9);
			SmartDashboard::PutNumber("Back Left Steer Offset", 140.8);
			SmartDashboard::PutNumber("Back Right Steer Offset", 0);
			m_intervalTimer.Reset();
			m_intervalTimer.Start();
			SmartDashboard::PutNumber("Front Left Steer Angle", m_leftFrontModule->getAngle());
						SmartDashboard::PutNumber("Front Right Steer Angle", m_rightFrontModule->getAngle());
						SmartDashboard::PutNumber("Back Left Steer Angle", m_leftBackModule->getAngle());
						SmartDashboard::PutNumber("Back Right Steer Angle", m_rightBackModule->getAngle());
	}

	void OperatorControl() {
		m_swerveDrive.setSteerPID(SmartDashboard::GetNumber("Steer kP Value", 0),
				SmartDashboard::GetNumber("Steer kI Value", 0),
				SmartDashboard::GetNumber("Steer kD Value", 0));
		m_leftFrontModule->setAngleOffset(SmartDashboard::GetNumber("Front Left Steer Offset", 0));
		m_rightFrontModule->setAngleOffset(SmartDashboard::GetNumber("Front Right Steer Offset", 0));
		m_leftBackModule->setAngleOffset(SmartDashboard::GetNumber("Back Left Steer Offset", 0));
		m_rightBackModule->setAngleOffset(SmartDashboard::GetNumber("Back Right Steer Offset", 0));
		while (IsOperatorControl() && IsEnabled()) {

			SmartDashboard::PutNumber("Front Left Steer Angle", m_leftFrontModule->getAngle());
						SmartDashboard::PutNumber("Front Right Steer Angle", m_rightFrontModule->getAngle());
						SmartDashboard::PutNumber("Back Left Steer Angle", m_leftBackModule->getAngle());
						SmartDashboard::PutNumber("Back Right Steer Angle", m_rightBackModule->getAngle());
			double rcw = abs(m_stick.GetRawAxis(4)) < 0.1 ? 0 : m_stick.GetRawAxis(4);
			double forward = abs(m_stick.GetY()) < 0.1 ? 0 : m_stick.GetY() * -1;
			double strafe = abs(m_stick.GetX()) < 0.1 ? 0 : m_stick.GetX();
			if(SmartDashboard::GetBoolean("Field Centric", false)) {
				double gyro_radians = toRadians(m_gyro->GetYaw());
				strafe = -forward * sin(gyro_radians) + strafe * cos(gyro_radians);
				forward = forward * cos(gyro_radians) + strafe * sin(gyro_radians);
			}
			m_swerveDrive.calculate(forward, strafe, rcw);
			m_swerveDrive.update();
			if(m_intervalTimer.Get() > 0.5) {
				cout << m_swerveDrive.print() << endl;
				m_intervalTimer.Reset();
				m_intervalTimer.Start();
			}
			Wait(kUpdatePeriod);
		}
	}

private:
	Joystick m_stick{0};
	static constexpr double kUpdatePeriod = 0.005;

	CANTalon m_leftFrontSteer, m_rightFrontSteer, m_leftBackSteer, m_rightBackSteer,
	            m_rightFrontDrive, m_leftFrontDrive, m_rightBackDrive, m_leftBackDrive;
	CORESwerve::SwerveModule *m_leftBackModule;
	CORESwerve::SwerveModule *m_rightBackModule;
	CORESwerve::SwerveModule *m_rightFrontModule;
	CORESwerve::SwerveModule *m_leftFrontModule;
	CORESwerve m_swerveDrive;
	shared_ptr<AHRS> m_gyro;
	CORETimer m_intervalTimer;
};

START_ROBOT_CLASS(Robot)
