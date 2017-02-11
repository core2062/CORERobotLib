#pragma once

#include "Rotation2d.h"

class Translation2d{
protected:
	double m_x, m_y;

public:
	Translation2d();

	Translation2d(double x, double y);

	double norm();

	double getX();

	double getY();

	void setX(double x);

	void setY(double y);

	Translation2d translateBy(Translation2d other);

	Translation2d rotateBy(Rotation2d rotation);

	Translation2d inverse();

	Translation2d interpolate(Translation2d other, double x);

	Translation2d extrapolate(Translation2d other, double x);


};
