#pragma once

#include "TankRotation2d.h"

class TankTranslation2d{
protected:
	double m_x, m_y;

public:
	TankTranslation2d();

	TankTranslation2d(double x, double y);

	double Norm();

	double GetX();

	double GetY();

	void SetX(double x);

	void SetY(double y);

	TankTranslation2d TranslateBy(TankTranslation2d other);

	TankTranslation2d RotateBy(TankRotation2d rotation);

	TankTranslation2d Inverse();

	TankTranslation2d Interpolate(TankTranslation2d other, double x);

	TankTranslation2d Extrapolate(TankTranslation2d other, double x);

	TankTranslation2d FlipX();
	TankTranslation2d FlipY();

};
