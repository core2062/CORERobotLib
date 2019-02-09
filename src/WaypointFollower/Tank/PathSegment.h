#pragma once

#include "Position2d.h"

class PathSegment{
public:
	struct Sample{
		Translation2d translation;
		double speed;
		Sample(Translation2d newTranslation, double newSpeed);
	};

protected:
	double m_speed;
	Translation2d m_start;
	Translation2d m_end;
	Translation2d m_startToEnd;
	double m_length;

public:
	struct ClosestPointReport{
		double index;
		double clampedIndex;
		Translation2d closestPoint;
		double distance;
	};

	PathSegment(Translation2d start, Translation2d end, double speed);

	void UpdateStart(Translation2d newStart);

	double GetSpeed();
	Translation2d GetStart();
	Translation2d GetEnd();
	double GetLength();

	Translation2d Interpolate(double index);
	double DotProduct(Translation2d other);
	ClosestPointReport GetClosestPoint(Translation2d queryPoint);

};