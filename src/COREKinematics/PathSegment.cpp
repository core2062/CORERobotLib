#include "PathSegment.h"

PathSegment::Sample::Sample(COREVector newTranslation, double newSpeed) {
    translation = newTranslation;
    speed = newSpeed;
}

PathSegment::PathSegment(COREVector start, COREVector end, double speed) {
    m_end = end;
    m_speed = speed;
    updateStart(start);
}

void PathSegment::updateStart(COREVector newStart) {
    m_start = newStart;
    m_startToEnd = m_start.MagnitudeInverse().TranslateBy(m_end);
    m_length = m_startToEnd.GetMagnitude();
//	std::cout << "New Length: " << m_length << std::endl;
}

double PathSegment::getSpeed() {
    return m_speed;
}

COREVector PathSegment::getStart() {
    return m_start;
}

COREVector PathSegment::getEnd() {
    return m_end;
}

double PathSegment::getLength() {
    return m_length;
}

COREVector PathSegment::interpolate(double index) {
    return m_start.InterpolateMagnitude(m_end, index);
}

double PathSegment::dotProduct(COREVector other) {
	COREVector startToOther = m_start.MagnitudeInverse().TranslateBy(other);
    return other.GetDotProduct(other);
}

PathSegment::ClosestPointReport PathSegment::getClosestPoint(COREVector queryPoint) {
    ClosestPointReport rv;
    if (m_length > kE) {
        double dot = dotProduct(queryPoint);
        rv.index = dot / (m_length * m_length);
        rv.clampedIndex = min(1.0, max(0.0, rv.index));
        rv.closestPoint = interpolate(rv.index);
    } else {
        rv.index = 0.0;
        rv.clampedIndex = 0.0;
        rv.closestPoint = COREVector(m_start);
    }
    rv.distance = rv.closestPoint.MagnitudeInverse().TranslateBy(queryPoint).GetMagnitude();
    return rv;
}
