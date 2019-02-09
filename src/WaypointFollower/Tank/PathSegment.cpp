#include "PathSegment.h"

PathSegment::Sample::Sample(Translation2d newTranslation, double newSpeed) {
	translation = newTranslation;
	speed = newSpeed;
}

PathSegment::PathSegment(Translation2d start, Translation2d end, double speed) {
	m_end = end;
	m_speed = speed;
	UpdateStart(start);
}

void PathSegment::UpdateStart(Translation2d newStart) {
	m_start = newStart;
	m_startToEnd = m_start.Inverse().TranslateBy(m_end);
	m_length = m_startToEnd.Norm();
//	std::cout << "New Length: " << m_length << std::endl;
}

double PathSegment::GetSpeed() {
	return m_speed;
}

Translation2d PathSegment::GetStart() {
	return m_start;
}

Translation2d PathSegment::GetEnd() {
	return m_end;
}

double PathSegment::GetLength() {
	return m_length;
}

Translation2d PathSegment::Interpolate(double index) {
	return m_start.Interpolate(m_end, index);
}

double PathSegment::DotProduct(Translation2d other) {
	Translation2d startToOther = m_start.Inverse().TranslateBy(other);
	return m_startToEnd.GetX() * startToOther.GetX() + m_startToEnd.GetY() * startToOther.GetY();
}

PathSegment::ClosestPointReport PathSegment::GetClosestPoint(Translation2d queryPoint) {
	ClosestPointReport rv;
	if (m_length > kE){
		double dot = DotProduct(queryPoint);
		rv.index = dot / (m_length * m_length);
		rv.clampedIndex = min(1.0, max(0.0, rv.index));
		rv.closestPoint = Interpolate(rv.index);
	} else {
		rv.index = 0.0;
		rv.clampedIndex = 0.0;
		rv.closestPoint = Translation2d(m_start);
	}
	rv.distance = rv.closestPoint.Inverse().TranslateBy(queryPoint).Norm();
	return rv;
}