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
	static Path FromFile(string fileName, bool flip);
	static Path FromText(string textPath, bool flip);
	Waypoint GetFirstWaypoint();
	double Update(Translation2d pos);
	bool EventPassed(string event);
	double GetRemainingLength();
	PathSegment::Sample GetLookaheadPoint(Translation2d pos, double lookahead);
	pair<bool, Translation2d> GetFirstCircleSegmentIntersection(PathSegment segment, Translation2d center,
																	 double radius);
	Position2d GetClosestPoint(Translation2d pos);
};
