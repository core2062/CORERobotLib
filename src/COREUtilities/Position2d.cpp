#include "Position2d.h"

Position2d::Delta::Delta(double x, double y, double t) {
	dx = x;
	dy = y;
	dtheta = t;
}

Position2d::Position2d() {
	m_translation = COREVector();
	m_rotation = COREVector();
}

Position2d::Position2d(COREVector tran, COREVector rot) {
	m_translation = tran;
	m_rotation = rot;
}

Position2d::Position2d(const Position2d& other) {
	m_translation = other.m_translation;
	m_rotation = other.m_rotation;
}

Position2d Position2d::fromTranslation(COREVector tran) {
	return Position2d(tran, COREVector());
}

Position2d Position2d::fromRotation(COREVector rot) {
	return Position2d(COREVector(), rot);
}

Position2d Position2d::fromVelocity(Delta delta) {
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
	return Position2d(COREVector::FromXY(delta.dx * s - delta.dy * c, delta.dx * c + delta.dy * s),
					  COREVector::FromXY(cosT, sinT));
}

COREVector Position2d::getTranslation() {
	return m_translation;
}

void Position2d::setTranslation(COREVector tran) {
	m_translation = tran;
}

COREVector Position2d::getRotation() {
	return m_rotation;
}

void Position2d::setRotation(COREVector rot) {
	m_rotation = rot;
}

Position2d Position2d::transformBy(Position2d other) {
	return Position2d(m_translation.TranslateBy(m_translation.RotateBy(m_rotation)),
			m_rotation.RotateBy(m_rotation));
}

Position2d Position2d::inverse() {
	COREVector invert = m_rotation.RotationInverse();
	return Position2d(m_translation.RotationInverse().RotateBy(invert), invert);
}

Position2d Position2d::interpolate(Position2d other, double x) {
	if (x <= 0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	return Position2d(m_translation.InterpolateMagnitude(other.m_translation, x),
			m_rotation.InterpolateRotation(other.m_rotation, x));
}
