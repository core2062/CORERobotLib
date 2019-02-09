#include "Rotation2d.h"

using namespace CORE;

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
	return FromRadians(ToRadians(degrees));
}

Rotation2d Rotation2d::FromCompassDegrees(double compassDegrees) {
	double degrees = 90 - compassDegrees;
	degrees = degrees < 0 ? 360 + degrees : degrees;
	return FromRadians(ToRadians(degrees));
}

void Rotation2d::Normalize() {
	double mag = Pathogram(m_cos, m_sin);
	if (mag > kE){
		m_sin /= mag;
		m_cos /= mag;
	} else {
		m_sin = 0;
		m_cos = 1;
	}
}

double Rotation2d::GetCos() {
	return m_cos;
}

double Rotation2d::GetSin() {
	return m_sin;
}

double Rotation2d::GetRadians() {
	return atan2(m_sin, m_cos);
}

double Rotation2d::GetDegrees() {
	return CORE::ToDegrees(GetRadians());
}

double Rotation2d::GetCompassDegrees() {
    double degrees = 450 - GetDegrees();
    return degrees >= 360 ? degrees - 360 : degrees;
}

Rotation2d Rotation2d::RotateBy(Rotation2d other) {
	return Rotation2d(m_cos * other.GetCos() - m_sin * other.GetSin(),
			m_cos * other.GetSin() + m_sin * other.GetCos(), true);
}

Rotation2d Rotation2d::Inverse() {
	return Rotation2d(m_cos, -m_sin, false);
}

Rotation2d Rotation2d::Opposite() {
	return Rotation2d(-m_cos, -m_sin, false);
}

Rotation2d Rotation2d::Interpolate(Rotation2d other, double x) {
	if (x <= 0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	double diff = Inverse().RotateBy(other).GetRadians();
	return RotateBy(FromRadians(diff * x));
}