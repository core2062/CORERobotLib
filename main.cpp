#include <iostream>
#include "CORERobotLib.h"
#include <map>

using namespace CORE;
using namespace std;
class DriveSubsystem : public CORE::CORESubsystem {
public:
    COREMotor<CANTalon> testMotor;
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
    }
    void teleop() {

    }
    void test() {
        cout << "Tested!" << endl;
    }
};

class robot : public CORERobot {

};

int main()
{
    
    cout << "End!" << endl;
    return 0;
}
