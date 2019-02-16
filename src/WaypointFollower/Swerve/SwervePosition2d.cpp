#include "SwervePosition2d.h"

SwervePosition2d::SwerveDelta::SwerveDelta(double x, double y, double t) {
	dx = x;
	dy = y;
	dtheta = t;
}

SwervePosition2d::SwervePosition2d() {
	m_translation = SwerveTranslation2d();
	m_rotation = SwerveRotation2d();
}

SwervePosition2d::SwervePosition2d(SwerveTranslation2d tran, SwerveRotation2d rot) {
	m_translation = tran;
	m_rotation = rot;
}

SwervePosition2d::SwervePosition2d(const SwervePosition2d& other) {
	m_translation = other.m_translation;
	m_rotation = other.m_rotation;
}

SwervePosition2d SwervePosition2d::FromTranslation(SwerveTranslation2d tran) {
	return SwervePosition2d(tran, SwerveRotation2d());
}

SwervePosition2d SwervePosition2d::FromRotation(SwerveRotation2d rot) {
	return SwervePosition2d(SwerveTranslation2d(), rot);
}

SwervePosition2d SwervePosition2d::FromVelocity(SwerveDelta delta) {
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
	return SwervePosition2d(SwerveTranslation2d(delta.dx * s - delta.dy * c, delta.dx * c + delta.dy * s),
			SwerveRotation2d(cosT, sinT, false));
}

SwerveTranslation2d SwervePosition2d::GetTranslation() {
	return m_translation;
}

void SwervePosition2d::SetTranslation(SwerveTranslation2d tran) {
	m_translation = tran;
}

SwerveRotation2d SwervePosition2d::GetRotation() {
	return m_rotation;
}

void SwervePosition2d::SetRotation(SwerveRotation2d rot) {
	m_rotation = rot;
}

SwervePosition2d SwervePosition2d::TransformBy(SwervePosition2d other) {
	return SwervePosition2d(m_translation.TranslateBy(other.m_translation.RotateBy(m_rotation)),
			m_rotation.RotateBy(other.m_rotation));
}

SwervePosition2d SwervePosition2d::Inverse() {
	SwerveRotation2d invert = m_rotation.Inverse();
	return SwervePosition2d(m_translation.Inverse().RotateBy(invert), invert);
}

SwervePosition2d SwervePosition2d::Interpolate(SwervePosition2d other, double x) {
	if (x <= 0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	return SwervePosition2d(m_translation.Interpolate(other.m_translation, x),
			m_rotation.Interpolate(other.m_rotation, x));
}
