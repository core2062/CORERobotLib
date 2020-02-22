#pragma once

#include "SwerveRotation2d.h"

class SwerveTranslation2d{
protected:
	double m_x, m_y;

public:
	SwerveTranslation2d();

	SwerveTranslation2d(double x, double y);

	double Norm();

	double GetX();

	double GetY();

	void SetX(double x);

	void SetY(double y);

	SwerveTranslation2d TranslateBy(SwerveTranslation2d other);

	SwerveTranslation2d RotateBy(SwerveRotation2d rotation);

	SwerveTranslation2d Inverse();

	SwerveTranslation2d Interpolate(SwerveTranslation2d other, double x);

	SwerveTranslation2d Extrapolate(SwerveTranslation2d other, double x);

	SwerveTranslation2d FlipX();
	SwerveTranslation2d FlipY();

	double GetSin();
	double GetCos();


};