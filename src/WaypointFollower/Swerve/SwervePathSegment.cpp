#include "SwervePathSegment.h"

SwervePathSegment::SwerveSample::SwerveSample(SwerveTranslation2d newTranslation, double newSpeed) {
	translation = newTranslation;
	speed = newSpeed;
}

SwervePathSegment::SwervePathSegment(SwerveTranslation2d start, SwerveTranslation2d end, 
	SwerveRotation2d angle, double speed) {
	m_end = end;
	m_speed = speed;
    m_angle = angle;
	UpdateStart(start);
}

void SwervePathSegment::UpdateStart(SwerveTranslation2d newStart) {
	m_start = newStart;
	m_startToEnd = m_start.Inverse().TranslateBy(m_end);
	m_length = m_startToEnd.Norm();
//	std::cout << "New Length: " << m_length << std::endl;
}

double SwervePathSegment::GetSpeed() {
	return m_speed;
}

SwerveTranslation2d SwervePathSegment::GetStart() {
	return m_start;
}

SwerveTranslation2d SwervePathSegment::GetEnd() {
	return m_end;
}

double SwervePathSegment::GetLength() {
	return m_length;
}

SwerveTranslation2d SwervePathSegment::Interpolate(double index) {
	return m_start.Interpolate(m_end, index);
}

double SwervePathSegment::DotProduct(SwerveTranslation2d other) {
	SwerveTranslation2d startToOther = m_start.Inverse().TranslateBy(other);
	return m_startToEnd.GetX() * startToOther.GetX() + m_startToEnd.GetY() * startToOther.GetY();
}

SwervePathSegment::SwerveClosestPointReport SwervePathSegment::GetClosestPoint(SwerveTranslation2d queryPoint) {
	SwerveClosestPointReport rv;
	if (m_length > kE){
		double dot = DotProduct(queryPoint);
		rv.index = dot / (m_length * m_length);
		rv.clampedIndex = min(1.0, max(0.0, rv.index));
		rv.closestPoint = Interpolate(rv.index);
	} else {
		rv.index = 0.0;
		rv.clampedIndex = 0.0;
		rv.closestPoint = SwerveTranslation2d(m_start);
	}
	rv.distance = rv.closestPoint.Inverse().TranslateBy(queryPoint).Norm();
	return rv;
}

SwerveRotation2d SwervePathSegment::GetAngle() {
    return m_angle;
}
