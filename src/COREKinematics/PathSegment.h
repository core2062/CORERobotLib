#pragma once

#include "COREUtilities/Position2d.h"
#include "COREUtilities/COREVector.h"

class PathSegment {
public:
    struct Sample {
        COREVector translation;
        double speed;

        Sample(COREVector newTranslation, double newSpeed);
    };

protected:
    double m_speed;
    COREVector m_start;
    COREVector m_end;
    COREVector m_startToEnd;
    double m_length;

public:
    struct ClosestPointReport {
        double index;
        double clampedIndex;
        COREVector closestPoint;
        double distance;
    };

    PathSegment(COREVector start, COREVector end, double speed);

    void updateStart(COREVector newStart);

    double getSpeed();

    COREVector getStart();

    COREVector getEnd();

    double getLength();

    COREVector interpolate(double index);

    double dotProduct(COREVector other);

    ClosestPointReport getClosestPoint(COREVector queryPoint);

};
