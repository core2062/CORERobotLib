#pragma once

#include "PathSegment.h"
#include "CORELogging/CORELog.h"
#include <vector>
#include <algorithm>

using namespace CORE;

struct Waypoint{
	Translation2d position;
	double speed;
    Rotation2d rotation;
	std::string event;
    Waypoint(Translation2d pos, Rotation2d rotation, double spd = 0.0, std::string completeEvent = "");
};

class Path{
protected:
	std::vector<Waypoint> m_waypoints;
	std::vector<PathSegment> m_segments;
	std::vector<std::string> m_events;
public:
	//Path();
	Path(std::vector<Waypoint> waypoints = {Waypoint(Translation2d(), Rotation2d(), 0)}, bool flip = false);

	Waypoint getFirstWaypoint();

	double update(Translation2d pos);

	bool eventPassed(std::string event);

	double getRemainingLength();

	PathSegment::Sample getLookaheadPoint(Translation2d pos, double lookahead);

	std::pair<bool, Translation2d> getFirstCircleSegmentIntersection(PathSegment segment, Translation2d center,
																	 double radius);

	Position2d getClosestPoint(Translation2d pos);
};
