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

	void updateStart(Translation2d newStart);

	double getSpeed();
	Translation2d getStart();
	Translation2d getEnd();
	double getLength();

	Translation2d interpolate(double index);
	double dotProduct(Translation2d other);
	ClosestPointReport getClosestPoint(Translation2d queryPoint);

};
