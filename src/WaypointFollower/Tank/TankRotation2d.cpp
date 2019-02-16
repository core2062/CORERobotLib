#include "TankRotation2d.h"

TankRotation2d::TankRotation2d() {
	m_cos = 1;
	m_sin = 0;
}

TankRotation2d::TankRotation2d(double x, double y, bool doNormalize) {
	m_cos = x;
	m_sin = y;
	if(doNormalize){
		Normalize();
	}
}

TankRotation2d::TankRotation2d(const TankRotation2d& other) {
	m_cos = other.m_cos;
	m_sin = other.m_sin;
}

TankRotation2d TankRotation2d::FromRadians(double radians) {
	return TankRotation2d(cos(radians), sin(radians), false);
}

TankRotation2d TankRotation2d::FromDegrees(double degrees) {
	return FromRadians(CORE::ToRadians(degrees));
}

void TankRotation2d::Normalize() {
	double mag = CORE::hypot(m_cos, m_sin);
	if (mag > kE){
		m_sin /= mag;
		m_cos /= mag;
	} else {
		m_sin = 0;
		m_cos = 1;
	}
}

double TankRotation2d::GetCos() {
	return m_cos;
}

double TankRotation2d::GetSin() {
	return m_sin;
}

double TankRotation2d::GetRadians() {
	return atan2(m_sin, m_cos);
}

double TankRotation2d::GetDegrees() {
	return CORE::ToDegrees(GetRadians());
}

TankRotation2d TankRotation2d::RotateBy(TankRotation2d other) {
	return TankRotation2d(m_cos * other.GetCos() - m_sin * other.GetSin(),
			m_cos * other.GetSin() + m_sin * other.GetCos(), true);
}

TankRotation2d TankRotation2d::Inverse() {
	return TankRotation2d(m_cos, -m_sin, false);
}

TankRotation2d TankRotation2d::Opposite() {
	return TankRotation2d(-m_cos, -m_sin, false);
}

TankRotation2d TankRotation2d::Interpolate(TankRotation2d other, double x) {
	if (x <= 0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	double diff = Inverse().RotateBy(other).GetRadians();
	return RotateBy(FromRadians(diff * x));
}
