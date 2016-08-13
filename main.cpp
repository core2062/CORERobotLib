#include <iostream>
#include "CORERobotLib.h"
#include <map>

using namespace CORE;
using namespace std;
class DriveSubsystem : public CORE::CORESubsystem {
public:
    COREMotor testMotor;
    DriveSubsystem():
        testMotor(0)
        {
            cout << "Subsystem Constructor" << endl;
        }
    void robotInit() {
        cout << "Robot Init!" << endl;
    }
    void teleopInit() {
        cout << "Robot Init!" << endl;
		testMotor.setP(0.01);
		testMotor.setControlMode(VELPID);
		testMotor.set(0.5);
		testMotor.setDeadband(0.1);
    }
    void teleop() {
//		testMotor.Set(1);
    }
    void test() {
        cout << "Tested!" << endl;
    }
};

class robot : public CORERobot {
	void robotInit() {}
	void teleopInit() {}
	void teleop() {}
};

int main()
{
	robot * robot1 = new robot;
	new DriveSubsystem;
	CORESubsystemsManager::teleopInit();
	while(true) {
		CORESubsystemsManager::teleop();
		robot1->waitLoopTime();
	}
    cout << "End!" << endl;
    return 0;
}
