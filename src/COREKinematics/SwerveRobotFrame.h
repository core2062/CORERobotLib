#pragma once

#include "SwerveTracker.h"

using namespace CORE;

class RobotFrame : public Position2d {
public:
    RobotFrame(double x, double y, double degrees);

    RobotFrame(Position2d offset);

    RobotFrame();

    Position2d getLatest(COREVector translation);

    Position2d getPos(double time, COREVector translation);

};
