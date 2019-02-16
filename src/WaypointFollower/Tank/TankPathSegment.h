#pragma once

#include "TankPosition2d.h"

class TankPathSegment{
public:
	struct TankSample{
		TankTranslation2d translation;
		double speed;
		TankSample(TankTranslation2d newTranslation, double newSpeed);
	};

protected:
	double m_speed;
	TankTranslation2d m_start;
	TankTranslation2d m_end;
	TankTranslation2d m_startToEnd;
	double m_length;

public:
	struct TankClosestPointReport{
		double index;
		double clampedIndex;
		TankTranslation2d closestPoint;
		double distance;
	};

	TankPathSegment(TankTranslation2d start, TankTranslation2d end, double speed);

	void UpdateStart(TankTranslation2d newStart);

	double GetSpeed();
	TankTranslation2d GetStart();
	TankTranslation2d GetEnd();
	double GetLength();

	TankTranslation2d Interpolate(double index);
	double DotProduct(TankTranslation2d other);
	TankClosestPointReport GetClosestPoint(TankTranslation2d queryPoint);

};