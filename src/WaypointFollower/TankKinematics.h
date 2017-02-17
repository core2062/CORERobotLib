#pragma once

#include "Position2d.h"

struct VelocityPair{
	double left;
	double right;
	VelocityPair(double l, double r);
};

class TankKinematics{
public:
	static Position2d::Delta forwardKinematics(double leftDelta, double rightDelta);

	static Position2d::Delta forwardKinematics(double leftDelta, double rightDelta, double deltaRads);

	static Position2d integrateForwardKinematics(Position2d pos, double leftDelta, double rightDelta, Rotation2d heading);

	static VelocityPair inverseKinematics(Position2d::Delta vel);
};
