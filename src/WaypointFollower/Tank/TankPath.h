#pragma once

#include "TankTranslation2d.h"
#include "TankPathSegment.h"
#include <vector>
#include <algorithm>

struct TankWaypoint{
	TankTranslation2d position;
	double speed;
	std::string event;
	TankWaypoint(TankTranslation2d pos = TankTranslation2d(0,0), double spd = 0.0, std::string completeEvent = "");
};

class TankPath{
protected:
	std::vector<TankWaypoint> m_waypoints;
	std::vector<TankPathSegment> m_segments;
	std::vector<std::string> m_events;

public:
	//Path();
	TankPath(std::vector<TankWaypoint> waypoints = {TankWaypoint(TankTranslation2d(),0)}, bool flipY = false, bool flipX = false);

	double Update(TankTranslation2d pos);

	bool EventPassed(std::string event);

	double GetRemainingLength();

	TankPathSegment::TankSample GetLookaheadPoint(TankTranslation2d pos, double lookahead);

	std::pair<bool, TankTranslation2d> GetFirstCircleSegmentIntersection(TankPathSegment segment, TankTranslation2d center,
			double radius);
};