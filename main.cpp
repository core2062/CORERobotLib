#include <iostream>
#include "CORERobotLib.h"
#include "src/COREHardware.h"
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
    COREMotor driveMotor;
    COREMotor steerMotor;
    DriveSubsystem():
        driveMotor(DRIVEMOTOR),
		steerMotor(STEERMOTOR)
        {

        }
    void robotInit() {
    }
    void teleopInit() {

    }
    void teleop() {
    	COREHardware::motor(DRIVEMOTOR)->Set(COREHardware::joystick(0)->getAxis(RIGHTSTICKY));
    	COREHardware::motor(STEERMOTOR)->Set(COREHardware::joystick(0)->getAxis(LEFTSTICKX));
    }
    void test() {
        cout << "Tested!" << endl;
    }
};

class Robot : public CORERobot {
public:
	DriveSubsystem driveSubsystem;
    COREJoystick joystick1;
	Robot():
		driveSubsystem(),
		joystick1(0)
	{

	}
	void robotInit() {}
	void teleopInit() {}
	void teleop() {}
};

#ifdef __arm__
START_ROBOT_CLASS(Robot);
#else
START_SIMULATED_ROBOT_CLASS(Robot);
#endif
