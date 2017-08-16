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
	std::cout << "Y: " << absPos.getTranslation().getY() << std::endl;
	absPos.setTranslation(Translation2d(absPos.getTranslation().getX() - getTranslation().getX(), absPos.getTranslation().getY() - getTranslation().getY()).rotateBy(getRotation()));
	absPos.setRotation(absPos.getRotation().rotateBy(getRotation()));
	return absPos;
}

Position2d RobotFrame::getPos(double time) {
	Position2d absPos = Position2d(TankTracker::GetInstance()->getFieldToVehicle(time));
	absPos.setTranslation(Translation2d(absPos.getTranslation().getX() - getTranslation().getX(), absPos.getTranslation().getY() - getTranslation().getY()));
	absPos.setRotation(absPos.getRotation().rotateBy(getRotation()));
	return absPos;
}
