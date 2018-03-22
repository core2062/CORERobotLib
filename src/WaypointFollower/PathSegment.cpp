#include "PathSegment.h"

PathSegment::Sample::Sample(Translation2d newTranslation, double newSpeed) {
	translation = newTranslation;
	speed = newSpeed;
}

PathSegment::PathSegment(Translation2d start, Translation2d end, Rotation2d angle, double speed) {
	m_end = end;
	m_speed = speed;
    m_angle = angle;
	updateStart(start);
}

void PathSegment::updateStart(Translation2d newStart) {
	m_start = newStart;
	m_startToEnd = m_start.inverse().translateBy(m_end);
	m_length = m_startToEnd.norm();
//	std::cout << "New Length: " << m_length << std::endl;
}

double PathSegment::getSpeed() {
	return m_speed;
}

Translation2d PathSegment::getStart() {
	return m_start;
}

Translation2d PathSegment::getEnd() {
	return m_end;
}

double PathSegment::getLength() {
	return m_length;
}

Translation2d PathSegment::interpolate(double index) {
	return m_start.interpolate(m_end, index);
}

double PathSegment::dotProduct(Translation2d other) {
	Translation2d startToOther = m_start.inverse().translateBy(other);
	return m_startToEnd.getX() * startToOther.getX() + m_startToEnd.getY() * startToOther.getY();
}

PathSegment::ClosestPointReport PathSegment::getClosestPoint(Translation2d queryPoint) {
	ClosestPointReport rv;
	if (m_length > kE){
		double dot = dotProduct(queryPoint);
		rv.index = dot / (m_length * m_length);
		rv.clampedIndex = min(1.0, max(0.0, rv.index));
		rv.closestPoint = interpolate(rv.index);
	} else {
		rv.index = 0.0;
		rv.clampedIndex = 0.0;
		rv.closestPoint = Translation2d(m_start);
	}
	rv.distance = rv.closestPoint.inverse().translateBy(queryPoint).norm();
	return rv;
}

Rotation2d PathSegment::getAngle() {
    return m_angle;
}
