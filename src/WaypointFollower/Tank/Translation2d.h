#pragma once

#include "Rotation2d.h"

class Translation2d{
protected:
	double m_x, m_y;

public:
	Translation2d();

	Translation2d(double x, double y);

	double Norm();

	double GetX();

	double GetY();

	void SetX(double x);

	void SetY(double y);

	Translation2d TranslateBy(Translation2d other);

	Translation2d RotateBy(Rotation2d rotation);

	Translation2d Inverse();

	Translation2d Interpolate(Translation2d other, double x);

	Translation2d Extrapolate(Translation2d other, double x);

	Translation2d FlipX();
	Translation2d FlipY();


};