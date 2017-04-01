#pragma once

#include "TankTracker.h"

using namespace CORE;

class RobotFrame : public Position2d{
public:
	RobotFrame(double x, double y, double degrees);
	RobotFrame(Position2d offset);

	Position2d getLatest();
	Position2d getPos(double time);

};
