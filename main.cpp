#include <iostream>
#include "CORERobotLib.h"
#include <map>

using namespace CORE;
using namespace COREHardware;
using namespace std;
class DriveSubsystem : public CORE::CORESubsystem
{
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

template<class T>
class robot : {
public:
    robot()
    {

    }
};



int main()
{
    robot * Robot = new robot();
    std::shared_ptr<CORESubsystem> pointer(new DriveSubsystem());
    Robot->subsystemMap[0] = pointer;
    Robot->subsystemMap[0]->teleopInit();
    //robot * Robot = new robot();
    CORESubsystemsManager::robotInit();
    //delete CORESubsystemsManager;
    cout << "End!" << endl;
    return 0;
}
