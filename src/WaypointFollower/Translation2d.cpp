#include "Translation2d.h"

Translation2d::Translation2d() {
	m_x = 0;
	m_y = 0;
}

Translation2d::Translation2d(double x, double y) {
	m_x = x;
	m_y = y;
}

double Translation2d::norm() {
	return CORE::hypot(m_x, m_y);
}

double Translation2d::getX() {
	return m_x;
}

double Translation2d::getY() {
	return m_y;
}

void Translation2d::setX(double x) {
	m_x = x;
}

void Translation2d::setY(double y) {
	m_y = y;
}

Translation2d Translation2d::translateBy(Translation2d other) {
	return Translation2d(m_x + other.getX(), m_y + other.getY());
}

Translation2d Translation2d::rotateBy(Rotation2d rotation) {
	return Translation2d(m_x * rotation.getCos() - m_y * rotation.getSin(),
			m_x * rotation.getSin() + m_y * rotation.getCos());
}

Translation2d Translation2d::inverse() {
	return Translation2d(-m_x, -m_y);
}

Translation2d Translation2d::interpolate(Translation2d other, double x) {
	if(x <=0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	return extrapolate(other, x);
}

Translation2d Translation2d::extrapolate(Translation2d other, double x) {
	return Translation2d(x * (other.getX() - m_x) + m_x, x * (other.getY() - m_y) + m_y);
}
