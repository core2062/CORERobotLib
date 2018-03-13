#include "Path.h"

using namespace CORE;

Waypoint::Waypoint(COREVector pos, double spd, std::string completeEvent) {
    position = pos;
    speed = spd;
    event = completeEvent;
}

Path::Path(std::vector<Waypoint> waypoints, bool flipY, bool flipX) {
    m_waypoints = waypoints;
    for (unsigned int i = 0; i < m_waypoints.size() - 1; ++i) {
        if (flipX && flipY) {
            m_segments.push_back(
                    PathSegment(m_waypoints[i].position.MagnitudeInverse(), m_waypoints[i + 1].position.MagnitudeInverse(),
                                m_waypoints[i].speed));
        } else if (flipX) {
            CORELog::logInfo("Flipped X"); 
            m_segments.push_back(
                    PathSegment(m_waypoints[i].position.FlipX(), m_waypoints[i + 1].position.FlipX(),
                                m_waypoints[i].speed));
        } else if (flipY) {
            CORELog::logInfo("Flipped Y"); 
            m_segments.push_back(
                    PathSegment(m_waypoints[i].position.FlipY(), m_waypoints[i + 1].position.FlipY(),
                        m_waypoints[i].speed));
        } else {
            m_segments.push_back(
                    PathSegment(m_waypoints[i].position, m_waypoints[i + 1].position, m_waypoints[i].speed));
        }
    }

    if (m_waypoints.size() > 0) {
        if (m_waypoints[0].event != "") {
            m_events.push_back(m_waypoints[0].event);
        }
        m_waypoints.erase(m_waypoints.begin());
    }
}

double Path::update(COREVector pos) {
    double rv = 0.0;
    for (unsigned int i = 0; i < m_segments.size(); i++) {
        PathSegment::ClosestPointReport closestPointReport = m_segments[i].getClosestPoint(pos);
        if (closestPointReport.index >= .99) {
            m_segments.erase(m_segments.begin() + i);
            if (m_waypoints.size() > 0) {
                if (m_waypoints[0].event != "") {
                    m_events.push_back(m_waypoints[0].event);
                }
                m_waypoints.erase(m_waypoints.begin());
            }
            return update(pos);
        } else {
            if (closestPointReport.index > 0.0) {
                m_segments[i].updateStart(closestPointReport.closestPoint);
            }

            rv = closestPointReport.distance;

            if (m_segments.size() > i + 1) {
                PathSegment::ClosestPointReport nextClosestPointReport = m_segments[i + 1].getClosestPoint(pos);
                if (nextClosestPointReport.index > 0
                    && nextClosestPointReport.index < .99
                    && nextClosestPointReport.distance < rv) {
                    m_segments[i + 1].updateStart(nextClosestPointReport.closestPoint);
                    rv = nextClosestPointReport.distance;
                    m_segments.erase(m_segments.begin() + i);
                    if (m_waypoints.size() > 0) {
                        if (m_waypoints[0].event != "") {
                            m_events.push_back(m_waypoints[0].event);
                        }
                        m_waypoints.erase(m_waypoints.begin());
                    }
                }
            }
            break;
        }
    }
    return rv;
}

bool Path::eventPassed(std::string event) {
    return (find(m_events.begin(), m_events.end(), event) != m_events.end());
}

double Path::getRemainingLength() {
    double length = 0.0;
    for (auto i: m_segments) {
        length += i.getLength();
    }
    return length;
}

PathSegment::Sample Path::getLookaheadPoint(COREVector pos, double lookahead) {
    if (m_segments.size() == 0) {
        return PathSegment::Sample(COREVector(), 0);
    }

    COREVector posInverse = pos.MagnitudeInverse();
    if (posInverse.TranslateBy(m_segments[0].getStart()).NormalizeMagnitude() >= lookahead) {
        return PathSegment::Sample(m_segments[0].getStart(), m_segments[0].getSpeed());
    }
    for (unsigned int i = 0; i < m_segments.size(); ++i) {
        PathSegment segment = m_segments[i];
        double distance = posInverse.TranslateBy(segment.getEnd()).NormalizeMagnitude();
        if (distance >= lookahead) {
            std::pair<bool, COREVector> intersectionPoint = getFirstCircleSegmentIntersection(segment, pos,
                                                                                                 lookahead);
            if (intersectionPoint.first) {
                return PathSegment::Sample(intersectionPoint.second, segment.getSpeed());
            } else {
                CORELog::logError("Error? Bad things happened");
            }
        }
    }

    PathSegment lastSegment = m_segments[m_segments.size() - 1];
    PathSegment newLastSegment = PathSegment(lastSegment.getStart(), lastSegment.interpolate(10000), 
                                             lastSegment.getSpeed());
    std::pair<bool, COREVector> intersectionPoint = getFirstCircleSegmentIntersection(newLastSegment, pos,
                                                                                         lookahead);
    if (intersectionPoint.first) {
        return PathSegment::Sample(intersectionPoint.second, lastSegment.getSpeed());
    } else {
        CORELog::logError("Error? Bad things happend");
        return PathSegment::Sample(lastSegment.getEnd(), lastSegment.getSpeed());
    }
}

std::pair<bool, COREVector> Path::getFirstCircleSegmentIntersection(PathSegment segment, COREVector center,
                                                                       double radius) {
    double x1 = segment.getStart().GetX() - center.GetX();
    double y1 = segment.getStart().GetY() - center.GetY();
    double x2 = segment.getEnd().GetX() - center.GetX();
    double y2 = segment.getEnd().GetY() - center.GetY();
    double dx = x2 - x1;
    double dy = y2 - y1;
    double drSquared = dx * dx + dy * dy;
    double det = x1 * y2 - x2 * y1;

    double discriminant = drSquared * radius * radius - det * det;
    if (discriminant < 0) {
        return {false, COREVector()};
    }

    double sqrtDiscriminant = sqrt(discriminant);
    COREVector posSolution = COREVector::FromXY(
            (det * dy + ((dy < 0) ? -1 : 1) * dx * sqrtDiscriminant) / drSquared + center.GetX(),
            (-det * dx + abs(dy) * sqrtDiscriminant) / drSquared + center.GetY());
    COREVector negSolution = COREVector::FromXY(
            (det * dy - ((dy < 0) ? -1 : 1) * dx * sqrtDiscriminant) / drSquared + center.GetX(),
            (-det * dx - abs(dy) * sqrtDiscriminant) / drSquared + center.GetY());

    double posDot = segment.dotProduct(posSolution);
    double negDot = segment.dotProduct(negSolution);
    if (posDot < 0 && negDot >= 0) {
        return {true, negSolution};
    } else if (posDot >= 0 && negDot < 0) {
        return {true, posSolution};
    } else {
        if (abs(posDot) <= abs(negDot)) {
            return {true, posSolution};
        } else {
            return {true, negSolution};
        }
    }
}
