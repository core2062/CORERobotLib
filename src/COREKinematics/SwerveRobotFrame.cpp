#include "RobotFrame.h"
#include "SwerveTracker.h"

RobotFrame::RobotFrame(double x, double y, double degrees) :
        Position2d(COREVector(x, y), COREVector::FromDegrees(degrees)) {
}

RobotFrame::RobotFrame(Position2d other) :
        Position2d(other) {}

RobotFrame::RobotFrame() :
        Position2d() {}

Position2d RobotFrame::getLatest(COREVector translation) {
    Position2d absPos = Position2d(SwerveTracker::GetInstance()->getLatestFieldToVehicle());
    absPos.setTranslation(COREVector(translation.SubtractVector(translation).RotateBy(getRotation())));
    absPos.setRotation(absPos.getRotation().RotateBy(getRotation()));
    return absPos;
}

Position2d RobotFrame::getPos(double time, COREVector translation) {
    Position2d absPos = Position2d(SwerveTracker::GetInstance()->getFieldToVehicle(time));
    absPos.setTranslation(COREVector(translation.SubtractVector(translation)));
    absPos.setRotation(absPos.getRotation().RotateBy(getRotation()));
    return absPos;
}
