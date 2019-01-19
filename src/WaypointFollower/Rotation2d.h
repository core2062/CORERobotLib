#pragma once

#include "COREUtilities/COREMath.h"

class Rotation2d{
protected:
	double m_cos, m_sin;

public:
	Rotation2d();
	Rotation2d(double x, double y, bool doNormalize);
	Rotation2d(const Rotation2d& other);

	static Rotation2d FromRadians(double radians);
	static Rotation2d FromDegrees(double degrees);

	void Normalize();

	double GetCos();
	double GetSin();

	double GetRadians();
	double GetDegrees();

	Rotation2d RotateBy(Rotation2d other);
	Rotation2d Inverse();
	Rotation2d Opposite();

	Rotation2d Interpolate(Rotation2d other, double x);
};
