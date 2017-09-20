#include <COREData/COREData.h>
#include <COREFramework/CORERobot.h>
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

class testRobot : public CORERobot {
public:
    void robotInit() override {

    }
    void teleopInit() override {

    }
    void teleop() override {
        CORELog::logInfo("looping");
    }
public:

};

int main() {
    CORE::COREDashboard dashboard;
    auto subsystem = new testSubsystem();

    testRobot robot;
    Wait(100);
}