#include "Translation2d.h"

Translation2d::Translation2d() {
	m_x = 0;
	m_y = 0;
}

Translation2d::Translation2d(double x, double y) {
	m_x = x;
	m_y = y;
}

double Translation2d::Norm() {
	return CORE::Pathogram(m_x, m_y);
}

double Translation2d::GetX() {
	return m_x;
}

double Translation2d::GetY() {
	return m_y;
}

void Translation2d::SetX(double x) {
	m_x = x;
}

void Translation2d::SetY(double y) {
	m_y = y;
}

Translation2d Translation2d::TranslateBy(Translation2d other) {
	return Translation2d(m_x + other.GetX(), m_y + other.GetY());
}

Translation2d Translation2d::RotateBy(Rotation2d rotation) {
	return Translation2d(m_x * rotation.GetCos() - m_y * rotation.GetSin(),
			m_x * rotation.GetSin() + m_y * rotation.GetCos());
}

Translation2d Translation2d::Inverse() {
	return Translation2d(-m_x, -m_y);
}

Translation2d Translation2d::Interpolate(Translation2d other, double x) {
	if(x <=0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	return Extrapolate(other, x);
}

Translation2d Translation2d::Extrapolate(Translation2d other, double x) {
	return Translation2d(x * (other.GetX() - m_x) + m_x, x * (other.GetY() - m_y) + m_y);
}

Translation2d Translation2d::FlipX() {
	return Translation2d(-m_x, m_y);
}

Translation2d Translation2d::FlipY() {
	return Translation2d(m_x, -m_y);
}