#pragma once

#include "PathSegment.h"
#include <vector>
#include <algorithm>

struct Waypoint{
	Translation2d position;
	double speed;
	std::string flag;
	Waypoint(Translation2d pos, double spd, std::string setFlag = "");
};

class Path{
protected:
	std::vector<Waypoint> m_waypoints;
	std::vector<PathSegment> m_segments;
	std::vector<std::string> m_flags;

public:
	Path(std::vector<Waypoint> waypoints);

	double update(Translation2d pos);

	bool isFlagSet(std::string flag);

	double getRemainingLength();

	PathSegment::Sample getLookaheadPoint(Translation2d pos, double lookahead);

	std::pair<bool, Translation2d> getFirstCircleSegmentIntersection(PathSegment segment, Translation2d center,
			double radius);
};
