#pragma once

#include "TankPosition2d.h"
#include "COREUtilities/COREConstant.h"

struct VelocityPair{
	double left;
	double right;
	VelocityPair(double l, double r);
};

class TankKinematics{
public:
	static TankPosition2d::TankDelta ForwardKinematics(double leftDelta, double rightDelta);

	static TankPosition2d::TankDelta ForwardKinematics(double leftDelta, double rightDelta, double deltaRads);

	static TankPosition2d IntegrateForwardKinematics(TankPosition2d pos, double leftDelta, double rightDelta, TankRotation2d heading);

	static VelocityPair InverseKinematics(TankPosition2d::TankDelta vel);

	static CORE::COREConstant<double> wheelDiameter;
	static CORE::COREConstant<double> scrubFactor;
};
