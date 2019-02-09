#pragma once

#include "Translation2d.h"
#include "PathSegment.h"
#include <vector>
#include <algorithm>

struct Waypoint{
	Translation2d position;
	double speed;
	std::string event;
	Waypoint(Translation2d pos = Translation2d(0,0), double spd = 0.0, std::string completeEvent = "");
};

class Path{
protected:
	std::vector<Waypoint> m_waypoints;
	std::vector<PathSegment> m_segments;
	std::vector<std::string> m_events;

public:
	//Path();
	Path(std::vector<Waypoint> waypoints = {Waypoint(Translation2d(),0)}, bool flipY = false, bool flipX = false);

	double Update(Translation2d pos);

	bool EventPassed(std::string event);

	double GetRemainingLength();

	PathSegment::Sample GetLookaheadPoint(Translation2d pos, double lookahead);

	std::pair<bool, Translation2d> GetFirstCircleSegmentIntersection(PathSegment segment, Translation2d center,
			double radius);
};