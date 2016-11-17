#include <iostream>
#include "CORERobotLib.h"
#include <map>
//#include "WPILib.h"

using namespace CORE;
using namespace std;

enum portAssignments {
	STEERMOTOR = 12,
	DRIVEMOTOR = 13
};

class DriveSubsystem : public CORESubsystem {
public:
    DriveSubsystem() {
    }
    void robotInit() {

    }
    void teleopInit() {
    }
    void teleop() {
    	//cout << DriverStation::GetInstance().GetBatteryVoltage() << std::endl;
    	Robot::motor(DRIVEMOTOR)->Set(Robot::joystick(0)->getAxis(RIGHTSTICKY));
    	Robot::motor(STEERMOTOR)->Set(Robot::joystick(0)->getAxis(LEFTSTICKY));
    	cout << "Steer Motor: " << endl;
    }
    void test() {
        cout << "Tested!" << endl;
    }
};

class offSeasonRobot : public CORERobot {
public:
	DriveSubsystem driveSubsystem;
    COREJoystick joystick1;
    COREMotor driveMotor;
    COREMotor steerMotor;
    offSeasonRobot():
		driveSubsystem(),
		joystick1(0),
        driveMotor(DRIVEMOTOR),
		steerMotor(STEERMOTOR)
	{
	}
	void robotInit() {}
	void teleopInit() {}
	void teleop() {}
};

#ifdef __arm__
START_ROBOT_CLASS(offSeasonRobot);
#else
START_SIMULATED_ROBOT_CLASS(offSeasonRobot);
#endif
