#pragma once

#include "COREUtilities/COREMath.h"

class SwerveRotation2d{
protected:
	double m_cos, m_sin;

public:
	SwerveRotation2d();
	SwerveRotation2d(double x, double y, bool doNormalize);
	SwerveRotation2d(const SwerveRotation2d& other);

	static SwerveRotation2d FromRadians(double radians);
	static SwerveRotation2d FromDegrees(double degrees);

	void Normalize();

	double GetCos();
	double GetSin();

	double GetRadians();
	double GetDegrees();

	SwerveRotation2d RotateBy(SwerveRotation2d other);
	SwerveRotation2d Inverse();
	SwerveRotation2d Opposite();

	SwerveRotation2d Interpolate(SwerveRotation2d other, double x);
};
