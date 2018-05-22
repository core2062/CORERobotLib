#include "Rotation2d.h"

Rotation2d::Rotation2d() {
	m_cos = 1;
	m_sin = 0;
}

Rotation2d::Rotation2d(double x, double y, bool doNormalize) {
	m_cos = x;
	m_sin = y;
	if(doNormalize){
		Normalize();
	}
}

Rotation2d::Rotation2d(const Rotation2d& other) {
	m_cos = other.m_cos;
	m_sin = other.m_sin;
}

Rotation2d Rotation2d::FromRadians(double radians) {
	return Rotation2d(cos(radians), sin(radians), false);
}

Rotation2d Rotation2d::FromDegrees(double degrees) {
	return FromRadians(CORE::ToRadians(degrees));
}

void Rotation2d::Normalize() {
	double mag = hypot(m_cos, m_sin);
	if (mag > kE){
		m_sin /= mag;
		m_cos /= mag;
	} else {
		m_sin = 0;
		m_cos = 1;
	}
}

double Rotation2d::getCos() {
	return m_cos;
}

double Rotation2d::GetSin() {
	return m_sin;
}

double Rotation2d::getRadians() {
	return atan2(m_sin, m_cos);
}

double Rotation2d::getDegrees() {
	return CORE::ToDegrees(getRadians());
}

Rotation2d Rotation2d::rotateBy(Rotation2d other) {
	return Rotation2d(m_cos * other.getCos() - m_sin * other.GetSin(),
			m_cos * other.GetSin() + m_sin * other.getCos(), true);
}

Rotation2d Rotation2d::inverse() {
	return Rotation2d(m_cos, -m_sin, false);
}

Rotation2d Rotation2d::opposite() {
	return Rotation2d(-m_cos, -m_sin, false);
}

Rotation2d Rotation2d::interpolate(Rotation2d other, double x) {
	if (x <= 0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	double diff = inverse().rotateBy(other).getRadians();
	return rotateBy(FromRadians(diff * x));
}
