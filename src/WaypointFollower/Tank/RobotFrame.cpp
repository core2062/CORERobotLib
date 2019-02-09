#include "RobotFrame.h"

RobotFrame::RobotFrame(double x, double y, double degrees) :
	Position2d(Translation2d(x,y), Rotation2d::FromDegrees(degrees)) {
}

RobotFrame::RobotFrame(Position2d other) : Position2d(other) {

}

RobotFrame::RobotFrame():
	Position2d(){
}

Position2d RobotFrame::GetLatest() {
	Position2d absPos = Position2d(TankTracker::GetInstance()->GetLatestFieldToVehicle());
	std::cout << "Y: " << absPos.GetTranslation().GetY() << std::endl;
	absPos.SetTranslation(Translation2d(absPos.GetTranslation().GetX() - GetTranslation().GetX(), 
        absPos.GetTranslation().GetY() - GetTranslation().GetY()).RotateBy(GetRotation()));
	absPos.SetRotation(absPos.GetRotation().RotateBy(GetRotation()));
	return absPos;
}

Position2d RobotFrame::GetPos(double time) {
	Position2d absPos = Position2d(TankTracker::GetInstance()->GetFieldToVehicle(time));
	absPos.SetTranslation(Translation2d(absPos.GetTranslation().GetX() - GetTranslation().GetX(), 
        absPos.GetTranslation().GetY() - GetTranslation().GetY()));
	absPos.SetRotation(absPos.GetRotation().RotateBy(GetRotation()));
	return absPos;
}