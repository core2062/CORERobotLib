#pragma once

#include "COREUtilities/COREMath.h"

class TankRotation2d{
protected:
	double m_cos, m_sin;

public:
	TankRotation2d();
	TankRotation2d(double x, double y, bool doNormalize);
	TankRotation2d(const TankRotation2d& other);

	static TankRotation2d FromRadians(double radians);
	static TankRotation2d FromDegrees(double degrees);
	static TankRotation2d FromCompassDegrees(double compassDegrees);

	void Normalize();

	double GetCos();
	double GetSin();

	double GetRadians();
	double GetDegrees();
	double GetCompassDegrees();

	TankRotation2d RotateBy(TankRotation2d other);
	TankRotation2d Inverse();
	TankRotation2d Opposite();

	TankRotation2d Interpolate(TankRotation2d other, double x);
};