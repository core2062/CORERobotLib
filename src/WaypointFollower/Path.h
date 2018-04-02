#pragma once

#include "PathSegment.h"
#include "CORELogging/CORELog.h"
#include <vector>
#include <algorithm>
#include "json.hpp"

using namespace CORE;
using namespace nlohmann;
using namespace std;

struct Waypoint {
	Translation2d position;
	double speed;
    Rotation2d rotation;
	string event;
    Waypoint(Translation2d pos, Rotation2d rotation, double spd = 0.0, string completeEvent = "");
};

class Path {
protected:
	vector<Waypoint> m_waypoints;
	vector<PathSegment> m_segments;
	vector<string> m_events;
public:
	Path();
	Path(vector<Waypoint> waypoints, bool flip = false);
	static Path fromFile(string fileName, bool flip);
	static Path fromText(string textPath, bool flip);
	Waypoint getFirstWaypoint();
	double update(Translation2d pos);
	bool eventPassed(string event);
	double getRemainingLength();
	PathSegment::Sample getLookaheadPoint(Translation2d pos, double lookahead);
	pair<bool, Translation2d> getFirstCircleSegmentIntersection(PathSegment segment, Translation2d center,
																	 double radius);
	Position2d getClosestPoint(Translation2d pos);
};
