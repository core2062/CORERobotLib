#pragma once

#include "SwervePosition2d.h"
#include "COREUtilities/COREConstant.h"

struct SwerveVelocityPair{
	double left;
	double right;
	SwerveVelocityPair(double l, double r);
};

class SwerveKinematics{
public:
	static SwervePosition2d::SwerveDelta ForwardKinematics(double leftDelta, double rightDelta);

	static SwervePosition2d::SwerveDelta ForwardKinematics(double leftDelta, double rightDelta, double deltaRads);

	static SwervePosition2d IntegrateForwardKinematics(SwervePosition2d pos, double leftDelta, double rightDelta, 
		SwerveRotation2d heading);

	static SwerveVelocityPair InverseKinematics(SwervePosition2d::SwerveDelta vel);

	static CORE::COREConstant<double> wheelDiameter;
	static CORE::COREConstant<double> scrubFactor;
};
