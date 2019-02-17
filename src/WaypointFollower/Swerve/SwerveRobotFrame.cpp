#include "SwerveRobotFrame.h"

SwerveRobotFrame::SwerveRobotFrame(double x, double y, double degrees) :
	SwervePosition2d(SwerveTranslation2d(x,y), SwerveRotation2d::FromDegrees(degrees)) {
}

SwerveRobotFrame::SwerveRobotFrame(SwervePosition2d other) : SwervePosition2d(other) {

}

SwerveRobotFrame::SwerveRobotFrame():
	SwervePosition2d(){
}

SwervePosition2d SwerveRobotFrame::GetLatest() {
	SwervePosition2d absPos = SwervePosition2d(SwerveTracker::GetInstance()->GetLatestFieldToVehicle());
	std::cout << "Y: " << absPos.GetTranslation().GetY() << std::endl;
	absPos.SetTranslation(SwerveTranslation2d(absPos.GetTranslation().GetX() - GetTranslation().GetX(), 
        absPos.GetTranslation().GetY() - GetTranslation().GetY()).RotateBy(GetRotation()));
	absPos.SetRotation(absPos.GetRotation().RotateBy(GetRotation()));
	return absPos;
}

SwervePosition2d SwerveRobotFrame::GetPos(double time) {
	SwervePosition2d absPos = SwervePosition2d(SwerveTracker::GetInstance()->GetFieldToVehicle(time));
	absPos.SetTranslation(SwerveTranslation2d(absPos.GetTranslation().GetX() - GetTranslation().GetX(), 
        absPos.GetTranslation().GetY() - GetTranslation().GetY()));
	absPos.SetRotation(absPos.GetRotation().RotateBy(GetRotation()));
	return absPos;
}