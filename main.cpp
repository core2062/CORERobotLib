#include <iostream>
#include "CORERobotLib.h"
#include <map>
//#include <WPILib.h>

using namespace CORE;
using namespace std;

enum portAssignments {
	STEERMOTOR = 15,
	DRIVEMOTOR = 16
};

class DriveSubsystem : public CORESubsystem {
public:
    DriveSubsystem() {
    }
    void robotInit() {

    }
    void teleopInit() {
    	cout << Robot::motor(0)->getPort();
    }
    void teleop() {
    	Robot::motor(0)->Set(Robot::joystick(0)->getAxis(RIGHTSTICKY));
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
    offSeasonRobot():
		driveSubsystem(),
		joystick1(0),
        driveMotor(1)
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
