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

class GearSubsystem {
public:
	enum JoystickButton {
		DPAD_N = -1,
		DPAD_NE = -2,
		DPAD_E = -3,
		DPAD_SE = -4,
		DPAD_S = -5,
		DPAD_SW = -6,
		DPAD_W = -7,
		DPAD_NW = -8,
		LEFT_STICK = 9,
		RIGHT_STICK = 10,
		BACK_BUTTON = 7,
		START_BUTTON = 8,
		Y_BUTTON = 4,
		B_BUTTON = 2,
		A_BUTTON = 1,
		X_BUTTON = 3,
		LEFT_BUTTON = 5,
		RIGHT_BUTTON = 6,
		LEFT_TRIGGER = 24,
		RIGHT_TRIGGER = 25
	};
	enum JoystickAxis {
			LEFT_STICK_X = 0,
			LEFT_STICK_Y = 1,
			RIGHT_STICK_X = 4,
			RIGHT_STICK_Y = 5,
			RIGHT_TRIGGER_AXIS = 3,
			LEFT_TRIGGER_AXIS = 2
		};
	enum class GearPickupState{
		HOLDING,
		FEEDING,
		PICKUP,
		PLACING,
		PICKINGUP,
		AUTOPLACING
	};
    GearPickupState m_state = GearPickupState::HOLDING;
    DoubleSolenoid m_manipulatorForwardSolenoid, m_manipulatorReverseSolenoid, m_leftFlapSolenoid, m_rightFlapSolenoid;
    CANTalon m_rollerMotor;
    double m_actuateTime, m_reverseRollerTime;
    CORETimer m_placeTimer;
    double oldIntakeButton = 0.0;
    bool lastRightButton = false;
	GearSubsystem() :
		m_manipulatorForwardSolenoid(GEAR_PICKUP_FORWARD_PCM, GEAR_PICKUP_FORWARD_ON_PORT, GEAR_PICKUP_FORWARD_OFF_PORT),
		m_manipulatorReverseSolenoid(GEAR_PICKUP_REVERSE_PCM, GEAR_PICKUP_REVERSE_ON_PORT, GEAR_PICKUP_REVERSE_OFF_PORT),
		m_leftFlapSolenoid(LEFT_GEAR_FLAP_SOLENOID_PCM, LEFT_GEAR_FLAP_SOLENOID_OUT_PORT, LEFT_GEAR_FLAP_SOLENOID_IN_PORT),
		m_rightFlapSolenoid(RIGHT_GEAR_FLAP_SOLENOID_PCM, RIGHT_GEAR_FLAP_SOLENOID_OUT_PORT, RIGHT_GEAR_FLAP_SOLENOID_IN_PORT),
		m_rollerMotor(GEAR_INTAKE_PORT),
		m_actuateTime(.2),
		m_reverseRollerTime(.05) {
		CORELog::logInfo("Gear subsystem constructor!");
	}

	void robotInit() {
		openFlap();

	}

	void teleopInit() {
		//	m_leftFlapSolenoid.Set(DoubleSolenoid::kForward);
		//	m_rightFlapSolenoid.Set(DoubleSolenoid::kForward);
		//	m_leftFlapSolenoid.Set(DoubleSolenoid::kReverse);
		//	m_rightFlapSolenoid.Set(DoubleSolenoid::kReverse);
	}

	void teleop(Joystick* joystick) {
		double intakeButton = joystick->GetRawButton(RIGHT_TRIGGER_AXIS);

		if(joystick->GetRawButton(RIGHT_BUTTON) && !lastRightButton){
			if(m_state == GearPickupState::HOLDING){
				placeGear();
			} else {
				holdIn();
			}
		} else if (intakeButton >= .1 && oldIntakeButton < .1){
			pickupGear();
		} else if (intakeButton < .1 && oldIntakeButton >= .1){
			safePickup();
		}  else if (joystick->GetRawAxis(LEFT_TRIGGER_AXIS) > .1){
			feed();
		} else if (m_state != GearPickupState::PICKUP && m_state != GearPickupState::PLACING && m_state != GearPickupState::PICKINGUP){
			holdIn();
		}
		lastRightButton = joystick->GetRawButton(RIGHT_BUTTON);

		oldIntakeButton = intakeButton;
	}

	void teleopEnd() {
		pickupOff();
	}

	void placeGear() {
		m_placeTimer.Reset();
		m_placeTimer.Start();
		m_state = GearPickupState::PLACING;
	}

	void autoPlaceGear() {
		m_placeTimer.Reset();
		m_placeTimer.Start();
		m_state = GearPickupState::AUTOPLACING;
	}

	void pickupGear() {
		m_placeTimer.Reset();
		m_placeTimer.Start();
		m_state = GearPickupState::PICKUP;
		std::cout << "PickupGear" << std::endl;
	}

	void holdIn() {
		m_state = GearPickupState::HOLDING;
	}

	void safePickup() {
		m_placeTimer.Reset();
		m_placeTimer.Start();
		m_state = GearPickupState::PICKINGUP;
	}

	void feed() {
		m_state = GearPickupState::FEEDING;
	}

	void pickupIn() {
		//	m_leftManipulatorSolenoid.Set(DoubleSolenoid::kReverse);
		//	m_rightManipulatorSolenoid.Set(DoubleSolenoid::kReverse);
		m_manipulatorForwardSolenoid.Set(DoubleSolenoid::kReverse);
		m_manipulatorReverseSolenoid.Set(DoubleSolenoid::kForward);
		//	m_out.Set(false);
		//	m_in.Set(true);
	}

	void pickupOut() {
		//	m_leftManipulatorSolenoid.Set(DoubleSolenoid::kForward);
		//	m_rightManipulatorSolenoid.Set(DoubleSolenoid::kForward);
		m_manipulatorReverseSolenoid.Set(DoubleSolenoid::kReverse);
		m_manipulatorForwardSolenoid.Set(DoubleSolenoid::kForward);
		//	m_in.Set(false);
		//	m_out.Set(true);
	}

	void pickupOff() {
		//	m_leftManipulatorSolenoid.Set(DoubleSolenoid::kOff);
		//	m_rightManipulatorSolenoid.Set(DoubleSolenoid::kOff);
		m_manipulatorReverseSolenoid.Set(DoubleSolenoid::kReverse);
		m_manipulatorForwardSolenoid.Set(DoubleSolenoid::kReverse);
	}

	void setRoller(double val) {
		m_rollerMotor.Set(val);
	}


	void openFlap(){
		m_leftFlapSolenoid.Set(DoubleSolenoid::kReverse);
		m_rightFlapSolenoid.Set(DoubleSolenoid::kReverse);
	}

	void closeFlap(){
		m_leftFlapSolenoid.Set(DoubleSolenoid::kForward);
		m_rightFlapSolenoid.Set(DoubleSolenoid::kForward);
	}

	bool flapIsOpen(){
		return (m_leftFlapSolenoid.Get() != DoubleSolenoid::kForward);
	}

	GearPickupState getState(){
		return m_state;
	}

	void postLoopTask(Joystick* joystick) {
		switch(m_state){
		case(GearPickupState::HOLDING):
				pickupIn();
		setRoller(0);
		break;
		case(GearPickupState::FEEDING):
				pickupIn();
		//		pickupOff();
		setRoller(-1.0);
		break;
		case(GearPickupState::PICKUP):
				if(m_placeTimer.Get() < m_actuateTime){
					pickupOut();
				} else {
					pickupOff();
				}
		//		pickupOut();
		setRoller(-1.0);
		break;
		case(GearPickupState::PLACING):
				if(m_placeTimer.Get() < m_reverseRollerTime){
					setRoller(0);
				} else {
					setRoller(1.0);
				}
		pickupOut();
		//		if(m_placeTimer.Get() > m_placeTime.Get()){
		//			m_state = GearPickupState::HOLDING;
		//		}
		break;
		case(GearPickupState::AUTOPLACING):
				if(m_placeTimer.Get() < m_reverseRollerTime){
					setRoller(0);
				} else {
					setRoller(1.0);
				}
		pickupOut();
		if(m_placeTimer.Get() > 1){
			m_state = GearPickupState::HOLDING;
		}
		break;
		case(GearPickupState::PICKINGUP):
				if(m_placeTimer.Get() < m_actuateTime*3){
					setRoller(-1.0);
				}else{
					holdIn();
				}
		pickupIn();
		break;
		}
	}
};

class Robot: public SampleRobot {
public:
	//GearSubsystem* pickup;
	Robot() :
		//pickup(),
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
		//pickup->robotInit();
		try {
			m_gyro = make_shared<AHRS>(SerialPort::Port::kUSB, AHRS::SerialDataType::kProcessedData, 80);
			CORELog::logInfo("NavX Initialized!");
		} catch(std::exception & ex) {
			CORELog::logWarning("Couldn't find NavX!");
		}
		SmartDashboard::PutNumber("Steer kP Value", 0.015);
		SmartDashboard::PutNumber("Steer kI Value", 0);
		SmartDashboard::PutNumber("Steer kD Value", 0);
		SmartDashboard::PutBoolean("Zero All Modules", false);
		SmartDashboard::PutBoolean("Field Centric", true);
		m_leftBackSteer.SetInverted(true);
		m_leftFrontSteer.SetInverted(true);
		m_rightBackSteer.SetInverted(true);
		m_rightFrontSteer.SetInverted(true);
		SmartDashboard::PutNumber("Front Left Steer Offset", 255.1);
		SmartDashboard::PutNumber("Front Right Steer Offset", 124);
		SmartDashboard::PutNumber("Back Left Steer Offset", 353.1);
		SmartDashboard::PutNumber("Back Right Steer Offset", 59.4);
		m_intervalTimer.Reset();
		m_intervalTimer.Start();
		SmartDashboard::PutNumber("Front Left Steer Angle", m_leftFrontModule->getAngle());
		SmartDashboard::PutNumber("Front Right Steer Angle", m_rightFrontModule->getAngle());
		SmartDashboard::PutNumber("Back Left Steer Angle", m_leftBackModule->getAngle());
		SmartDashboard::PutNumber("Back Right Steer Angle", m_rightBackModule->getAngle());
		m_gyro->ZeroYaw();
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
			//pickup->teleop(&m_stick);
			SmartDashboard::PutNumber("Gyro Yaw", m_gyro->GetYaw());
			SmartDashboard::PutNumber("Front Left Steer Angle", m_leftFrontModule->getAngle());
			SmartDashboard::PutNumber("Front Right Steer Angle", m_rightFrontModule->getAngle());
			SmartDashboard::PutNumber("Back Left Steer Angle", m_leftBackModule->getAngle());
			SmartDashboard::PutNumber("Back Right Steer Angle", m_rightBackModule->getAngle());
			double rcw = abs(m_stick.GetRawAxis(4)) < 0.1 ? 0 : m_stick.GetRawAxis(4);
			double forward = abs(m_stick.GetY()) < 0.1 ? 0 : m_stick.GetY() * -1;
			double strafe = abs(m_stick.GetX()) < 0.1 ? 0 : m_stick.GetX();
			if(true) {
				double gyro_radians = toRadians(m_gyro->GetYaw());
				double temp = forward * cos(gyro_radians) + strafe * sin(gyro_radians);
				strafe = -forward * sin(gyro_radians) + strafe * cos(gyro_radians);
				forward = temp;
			}
			m_swerveDrive.calculate(forward, strafe, rcw);
			m_swerveDrive.update();
			if(m_intervalTimer.Get() > 0.5) {
				cout << m_swerveDrive.print() << endl;
				m_intervalTimer.Reset();
				m_intervalTimer.Start();
			}
			//pickup->postLoopTask(&m_stick);
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
