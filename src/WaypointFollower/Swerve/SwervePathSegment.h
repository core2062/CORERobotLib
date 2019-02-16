#pragma once

#include "SwervePosition2d.h"

class SwervePathSegment{
public:
	struct SwerveSample{
		SwerveTranslation2d translation;
		double speed;
		SwerveSample(SwerveTranslation2d newTranslation, double newSpeed);
	};

protected:
	double m_speed;
	SwerveTranslation2d m_start;
	SwerveTranslation2d m_end;
	SwerveTranslation2d m_startToEnd;
	SwerveRotation2d m_angle;
	double m_length;

public:
	struct SwerveClosestPointReport{
		double index;
		double clampedIndex;
		SwerveTranslation2d closestPoint;
		double distance;
	};

	SwervePathSegment(SwerveTranslation2d start, SwerveTranslation2d end, SwerveRotation2d angle, double speed);

	void UpdateStart(SwerveTranslation2d newStart);

	double GetSpeed();
	SwerveTranslation2d GetStart();
	SwerveTranslation2d GetEnd();
	double GetLength();
	SwerveRotation2d GetAngle();

	SwerveTranslation2d Interpolate(double index);
	double DotProduct(SwerveTranslation2d other);
	SwerveClosestPointReport GetClosestPoint(SwerveTranslation2d queryPoint);
};
