#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "COREFramework/CORERobot.h"
#include "CORELogging/CORELog.h"

using namespace CORE;

TEST(COREFramework, GameStateCorrectlyTransitions) {
    class Robot : public CORERobot {
    public:
        Robot() {
            CORELog::logInfo("Robot");
        }
        ~Robot() {
            CORELog::logInfo("~Robot");
        }
        void robotInit() {
            CORELog::logInfo("RobotInit");
        }

        void teleopInit() {
            CORELog::logInfo("TeleopInit");
        }

        void teleop() {
            CORELog::logInfo("Teleop");
        }
    };
    Robot testRobot;
    CORELog::logInfo("Robot Finished Init");
    testRobot.robotInit();
}
