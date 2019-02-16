#include "TankTranslation2d.h"

TankTranslation2d::TankTranslation2d() {
	m_x = 0;
	m_y = 0;
}

TankTranslation2d::TankTranslation2d(double x, double y) {
	m_x = x;
	m_y = y;
}

double TankTranslation2d::Norm() {
	return CORE::hypot(m_x, m_y);
}

double TankTranslation2d::GetX() {
	return m_x;
}

double TankTranslation2d::GetY() {
	return m_y;
}

void TankTranslation2d::SetX(double x) {
	m_x = x;
}

void TankTranslation2d::SetY(double y) {
	m_y = y;
}

TankTranslation2d TankTranslation2d::TranslateBy(TankTranslation2d other) {
	return TankTranslation2d(m_x + other.GetX(), m_y + other.GetY());
}

TankTranslation2d TankTranslation2d::RotateBy(TankRotation2d rotation) {
	return TankTranslation2d(m_x * rotation.GetCos() - m_y * rotation.GetSin(),
			m_x * rotation.GetSin() + m_y * rotation.GetCos());
}

TankTranslation2d Translation2d::Inverse() {
	return TankTranslation2d(-m_x, -m_y);
}

TankTranslation2d TankTranslation2d::Interpolate(TankTranslation2d other, double x) {
	if(x <=0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	return Extrapolate(other, x);
}

TankTranslation2d TankTranslation2d::Extrapolate(TankTranslation2d other, double x) {
	return TankTranslation2d(x * (other.GetX() - m_x) + m_x, x * (other.GetY() - m_y) + m_y);
}

TankTranslation2d TankTranslation2d::FlipX() {
	return TankTranslation2d(-m_x, m_y);
}

TankTranslation2d TankTranslation2d::FlipY() {
	return TankTranslation2d(m_x, -m_y);
}
