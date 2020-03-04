#include "SwerveRotation2d.h"

SwerveRotation2d::SwerveRotation2d() {
	m_cos = 1;
	m_sin = 0;
}

SwerveRotation2d::SwerveRotation2d(double x, double y, bool doNormalize) {
	m_cos = x;
	m_sin = y;
	if(doNormalize){
		Normalize();
	}
}

SwerveRotation2d::SwerveRotation2d(const SwerveRotation2d& other) {
	m_cos = other.m_cos;
	m_sin = other.m_sin;
}

SwerveRotation2d SwerveRotation2d::FromRadians(double radians) {
	return SwerveRotation2d(cos(radians), sin(radians), false);
}

SwerveRotation2d SwerveRotation2d::FromDegrees(double degrees) {
	return FromRadians(CORE::ToRadians(degrees));
}

void SwerveRotation2d::Normalize() {
	double mag = hypot(m_cos, m_sin);
	if (mag > kE){
		m_sin /= mag;
		m_cos /= mag;
	} else {
		m_sin = 0;
		m_cos = 1;
	}
}

double SwerveRotation2d::GetCos() {
	return m_cos;
}

double SwerveRotation2d::GetSin() {
	return m_sin;
}

double SwerveRotation2d::GetRadians() {
	return atan2(m_sin, m_cos);
}

double SwerveRotation2d::GetDegrees() {
	return CORE::ToDegrees(GetRadians());
}

SwerveRotation2d SwerveRotation2d::RotateBy(SwerveRotation2d other) {
	return SwerveRotation2d(m_cos * other.GetCos() - m_sin * other.GetSin(),
			m_cos * other.GetSin() + m_sin * other.GetCos(), true);
}

SwerveRotation2d SwerveRotation2d::Inverse() {
	return SwerveRotation2d(m_cos, -m_sin, false);
}

SwerveRotation2d SwerveRotation2d::Opposite() {
	return SwerveRotation2d(-m_cos, -m_sin, false);
}

SwerveRotation2d SwerveRotation2d::Interpolate(SwerveRotation2d other, double x) {
	if (x <= 0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	double diff = Inverse().RotateBy(other).GetRadians();
	return RotateBy(FromRadians(diff * x));
}