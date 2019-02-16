#pragma once

#include "SwerveTracker.h"

using namespace CORE;

class SwerveRobotFrame : public SwervePosition2d{
public:
	SwerveRobotFrame(double x, double y, double degrees);
	SwerveRobotFrame(SwervePosition2d offset);
	SwerveRobotFrame();

	SwervePosition2d GetLatest();
	SwervePosition2d GetPos(double time);

};