#pragma once

#include "TankTracker.h"

using namespace CORE;

class TankRobotFrame : public TankPosition2d{
public:
	TankRobotFrame(double x, double y, double degrees);
	TankRobotFrame(TankPosition2d offset);
	TankRobotFrame();

	TankPosition2d GetLatest();
	TankPosition2d GetPos(double time);

};