#include "TankPosition2d.h"

TankPosition2d::TankDelta::TankDelta(double x, double y, double t) {
	dx = x;
	dy = y;
	dtheta = t;
}

TankPosition2d::TankPosition2d() {
	m_translation = TankTranslation2d();
	m_rotation = TankRotation2d();
}

TankPosition2d::TankPosition2d(TankTranslation2d tran, TankRotation2d rot) {
	m_translation = tran;
	m_rotation = rot;
}

TankPosition2d::TankPosition2d(const TankPosition2d& other) {
	m_translation = other.m_translation;
	m_rotation = other.m_rotation;
}

TankPosition2d TankPosition2d::FromTranslation(TankTranslation2d tran) {
	return TankPosition2d(tran, TankRotation2d());
}

TankPosition2d TankPosition2d::FromRotation(TankRotation2d rot) {
	return TankPosition2d(TankTranslation2d(), rot);
}

TankPosition2d TankPosition2d::FromVelocity(TankDelta delta) {
	double sinT = sin(delta.dtheta);
	double cosT = cos(delta.dtheta);
	double s, c;
	if(fabs(delta.dtheta) < kE){
		s = 1.0 - 1.0 / 6.0 * delta.dtheta * delta.dtheta;
		c = .5 * delta.dtheta;
	} else {
		s = sinT / delta.dtheta;
		c = (1.0 - cosT) / delta.dtheta;
	}
	return TankPosition2d(TankTranslation2d(delta.dx * s - delta.dy * c, delta.dx * c + delta.dy * s),
			TankRotation2d(cosT, sinT, false));
}

TankTranslation2d TankPosition2d::GetTranslation() {
	return m_translation;
}

void TankPosition2d::SetTranslation(TankTranslation2d tran) {
	m_translation = tran;
}

TankRotation2d TankPosition2d::GetRotation() {
	return m_rotation;
}

void TankPosition2d::SetRotation(TankRotation2d rot) {
	m_rotation = rot;
}

TankPosition2d TankPosition2d::TransformBy(TankPosition2d other) {
	return TankPosition2d(m_translation.TranslateBy(other.m_translation.RotateBy(m_rotation)),
			m_rotation.RotateBy(other.m_rotation));
}

TankPosition2d TankPosition2d::Inverse() {
	TankRotation2d invert = m_rotation.Inverse();
	return TankPosition2d(m_translation.Inverse().RotateBy(invert), invert);
}

TankPosition2d TankPosition2d::Interpolate(TankPosition2d other, double x) {
	if (x <= 0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	return TankPosition2d(m_translation.Interpolate(other.m_translation, x),
			m_rotation.Interpolate(other.m_rotation, x));
}
