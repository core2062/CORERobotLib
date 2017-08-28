#include <unistd.h>
#include <COREData/COREData.h>
#include <cxxabi.h>
#include "COREDashboard/COREDashboard.h"
#include "CORESimulation/CORESimRobot.h"

class testSubsystem : public CORESubsystem {
public:
    testSubsystem() : data("test group", "test", true), data2("test group", "testVal"), testVal(0) {

    }

    void robotInit() {

    }

    void teleopInit() {

    }

    void teleop() {
        data.Set(testVal);
        data2.Set("Yo " + to_string(testVal));
        testVal++;
    }

private:
    CORE::COREData<int> data;
    CORE::COREData<string> data2;
    int testVal;
};

int main() {
    CORE::COREDashboard dashboard;
    auto subsystem = new testSubsystem();

    CORE::SIM::CORESimRobot robot;
    robot.OperatorControl();
    Wait(100);
}