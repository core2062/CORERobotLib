#include "RobotFrame.h"

RobotFrame::RobotFrame(double x, double y, double degrees) :
	Position2d(Translation2d(x,y), Rotation2d::fromDegrees(degrees)) {
}

RobotFrame::RobotFrame(Position2d other) :
		Position2d(other)
{}

RobotFrame::RobotFrame():
		Position2d(){}

Position2d RobotFrame::getLatest() {
	Position2d absPos = Position2d(TankTracker::GetInstance()->getLatestFieldToVehicle());
	return absPos.transformBy(*this);
}

Position2d RobotFrame::getPos(double time) {
	Position2d absPos = Position2d(TankTracker::GetInstance()->getFieldToVehicle(time));
	return absPos.transformBy(*this);
}
