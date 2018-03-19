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
    bool angleProvided;
	std::string event;
    Waypoint(Translation2d pos, double spd, std::string completeEvent = "");
    Waypoint(Translation2d pos, Rotation2d rotation, double spd = 0.0, std::string completeEvent = "");
};

class Path{
protected:
	std::vector<Waypoint> m_waypoints;
	std::vector<PathSegment> m_segments;
	std::vector<std::string> m_events;
    Rotation2d m_lastRotation, m_nextRotation;
public:
	//Path();
	Path(std::vector<Waypoint> waypoints = {Waypoint(Translation2d(), 0)}, bool flipY = false, bool flipX = false);

	Waypoint getFirstWaypoint();

	double update(Translation2d pos);

	bool eventPassed(std::string event);

	double getRemainingLength();

	PathSegment::Sample getLookaheadPoint(Translation2d pos, double lookahead);

    Rotation2d getNextRotation();
    Rotation2d getLastRotation();

	std::pair<bool, Translation2d> getFirstCircleSegmentIntersection(PathSegment segment, Translation2d center,
			double radius);

    void flipX();
    void flipY();
};
