#include "TankRobotFrame.h"

TankRobotFrame::TankRobotFrame(double x, double y, double degrees) :
	TankPosition2d(TankTranslation2d(x,y), TankRotation2d::FromDegrees(degrees)) {
	cout<<"TankRobotFrame::TankRobotFrame"<<endl;
}

TankRobotFrame::TankRobotFrame(TankPosition2d other) :
		TankPosition2d(other){}

TankRobotFrame::TankRobotFrame():
		TankPosition2d(){}

TankPosition2d TankRobotFrame::GetLatest() {
	cout<<"TankPosition2d_TankRobotFrame"<<endl;
	TankPosition2d absPos = TankPosition2d(TankTracker::GetInstance()->GetLatestFieldToVehicle());
	std::cout << "Y: " << absPos.GetTranslation().GetY() << std::endl;
	absPos.SetTranslation(TankTranslation2d(absPos.GetTranslation().GetX() - GetTranslation().GetX(), 
		absPos.GetTranslation().GetY() - GetTranslation().GetY()).RotateBy(GetRotation()));
	absPos.SetRotation(absPos.GetRotation().RotateBy(GetRotation()));
	return absPos;
}

TankPosition2d TankRobotFrame::GetPos(double time) {
	cout<<"TankRobotFrame::GetPos"<<endl;
	TankPosition2d absPos = TankPosition2d(TankTracker::GetInstance()->GetFieldToVehicle(time));
	absPos.SetTranslation(TankTranslation2d(absPos.GetTranslation().GetX() - GetTranslation().GetX(), 
		absPos.GetTranslation().GetY() - GetTranslation().GetY()));
	absPos.SetRotation(absPos.GetRotation().RotateBy(GetRotation()));
	return absPos;
}
