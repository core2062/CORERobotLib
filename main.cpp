#include <iostream>
#include "CORERobotLib.h"
#include <map>
//#include <WPILib.h>

using namespace CORE;
using namespace std;
class DriveSubsystem : public CORESubsystem {
public:
    COREMotor testMotor;
    DriveSubsystem():
        testMotor(11)
        {
            cout << "Subsystem Constructor" << endl;
        }
    void robotInit() {
        cout << "Robot Init!" << endl;
    }
    void teleopInit() {
        cout << "WARNING: Teleop Init!" << endl;
//		testMotor.setP(0.01);
//		testMotor.setControlMode(VELPID);
//		testMotor.setVel(0.5);
//		testMotor.setDeadband(0.1);
    }
    void teleop() {
    	testMotor.setControlMode(controlMode::PERCENTAGE);
    	testMotor.setP(0.01);
    	testMotor.setControlMode(VELPID);
    	testMotor.setVel(0.5);
    	testMotor.setDeadband(0.1);
		testMotor.Set(0.7);
    }
    void test() {
        cout << "Tested!" << endl;
    }
};

class Robot : public CORERobot {
public:
	DriveSubsystem driveSubsystem;
	Robot():
		driveSubsystem()
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
