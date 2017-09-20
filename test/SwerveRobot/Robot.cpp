#include "Robot.h"

SwerveRobot * Robot = nullptr;

SwerveRobot::SwerveRobot() :
        driverJoystick(0),
        operatorJoystick(1) {
    Robot = this;
}

void SwerveRobot::robotInit() {
    setLoopTime(.025);
}

void SwerveRobot::teleopInit() {

}

void SwerveRobot::teleop() {

}

SwerveRobot::~SwerveRobot() {
    delete Robot;
    Robot = nullptr;
    CORELog::logInfo("Robot successfully deleted");
}

//START_ROBOT_CLASS(SwerveRobot)
