#pragma once

#include "PathSegment.h"
#include <vector>
#include <algorithm>
#include "CORELogging/CORELog.h"

struct Waypoint {
    COREVector position;
    double speed;
    std::string event;

    Waypoint(COREVector pos = COREVector(0, 0), double spd = 0.0, std::string completeEvent = "");
};

class Path {
protected:
    std::vector<Waypoint> m_waypoints;
    std::vector<PathSegment> m_segments;
    std::vector<std::string> m_events;

public:
    //Path();
    Path(std::vector<Waypoint> waypoints = {Waypoint(COREVector(), 0)}, bool flipY = false, bool flipX = false);

    double update(COREVector pos);

    bool eventPassed(std::string event);

    double getRemainingLength();

    PathSegment::Sample getLookaheadPoint(COREVector pos, double lookahead);

    std::pair<bool, COREVector> getFirstCircleSegmentIntersection(PathSegment segment, COREVector center,
                                                                     double radius);
};
