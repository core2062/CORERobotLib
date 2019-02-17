#pragma once

#include "SwervePathSegment.h"
#include "CORELogging/CORELog.h"
#include <vector>
#include <algorithm>
#include "json.hpp"

using namespace CORE;
using namespace nlohmann;
using namespace std;

struct SwerveWaypoint {
	SwerveTranslation2d position;
	double speed;
    SwerveRotation2d rotation;
	string event;
    SwerveWaypoint(SwerveTranslation2d pos, SwerveRotation2d rotation, double spd = 0.0, string completeEvent = "");
};

class SwervePath {
protected:
	vector<SwerveWaypoint> m_waypoints;
	vector<SwervePathSegment> m_segments;
	vector<string> m_events;
public:
	SwervePath();
	SwervePath(vector<SwerveWaypoint> waypoints, bool flip = false);
	static SwervePath FromFile(string fileName, bool flip);
	static SwervePath FromText(string textPath, bool flip);
	SwerveWaypoint GetFirstWaypoint();
	double Update(SwerveTranslation2d pos);
	bool EventPassed(string event);
	double GetRemainingLength();
	SwervePathSegment::SwerveSample GetLookaheadPoint(SwerveTranslation2d pos, double lookahead);
	pair<bool, SwerveTranslation2d> GetFirstCircleSegmentIntersection(SwervePathSegment segment, SwerveTranslation2d center,
																	 double radius);
	SwervePosition2d GetClosestPoint(SwerveTranslation2d pos);
};
