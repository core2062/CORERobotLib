#include "TankPathSegment.h"

TankPathSegment::TankSample::TankSample(TankTranslation2d newTranslation, double newSpeed) {
	translation = newTranslation;
	speed = newSpeed;
}

TankPathSegment::TankPathSegment(TankTranslation2d start, TankTranslation2d end, double speed) {
	m_end = end;
	m_speed = speed;
	UpdateStart(start);
}

void TankPathSegment::UpdateStart(TankTranslation2d newStart) {
	m_start = newStart;
	m_startToEnd = m_start.Inverse().TranslateBy(m_end);
	m_length = m_startToEnd.Norm();
//	std::cout << "New Length: " << m_length << std::endl;
}

double TankPathSegment::GetSpeed() {
	return m_speed;
}

TankTranslation2d TankPathSegment::GetStart() {
	return m_start;
}

TankTranslation2d TankPathSegment::GetEnd() {
	return m_end;
}

double TankPathSegment::GetLength() {
	return m_length;
}

TankTranslation2d TankPathSegment::Interpolate(double index) {
	return m_start.Interpolate(m_end, index);
}

double TankPathSegment::DotProduct(TankTranslation2d other) {
	TankTranslation2d startToOther = m_start.Inverse().TranslateBy(other);
	return m_startToEnd.GetX() * startToOther.GetX() + m_startToEnd.GetY() * startToOther.GetY();
}

TankPathSegment::TankClosestPointReport TankPathSegment::GetClosestPoint(TankTranslation2d queryPoint) {
	TankClosestPointReport rv;
	if (m_length > kE){
		double dot = DotProduct(queryPoint);
		rv.index = dot / (m_length * m_length);
		rv.clampedIndex = min(1.0, max(0.0, rv.index));
		rv.closestPoint = Interpolate(rv.index);
	} else {
		rv.index = 0.0;
		rv.clampedIndex = 0.0;
		rv.closestPoint = TankTranslation2d(m_start);
	}
	rv.distance = rv.closestPoint.Inverse().TranslateBy(queryPoint).Norm();
	return rv;
}