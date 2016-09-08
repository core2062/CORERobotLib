#include <iostream>
#include "CORERobotLib.h"
#include <map>

using namespace CORE;
using namespace std;
class DriveSubsystem : public CORESubsystem {
public:
    COREMotor testMotor;
    DriveSubsystem():
        testMotor(13)
        {
            cout << "Subsystem Constructor" << endl;
        }
    void robotInit() {
        cout << "Robot Init!" << endl;
    }
    void teleopInit() {
        cout << "Teleop Init!" << endl;
//		testMotor.setP(0.01);
//		testMotor.setControlMode(VELPID);
//		testMotor.setVel(0.5);
//		testMotor.setDeadband(0.1);
    }
    void teleop() {
    	testMotor.setControlMode(controlMode::PERCENTAGE);
		testMotor.Set(0.25);
    }
    void test() {
        cout << "Tested!" << endl;
    }
};

class Robot : public CORERobot {
public:
	Robot() {
		new DriveSubsystem();
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
