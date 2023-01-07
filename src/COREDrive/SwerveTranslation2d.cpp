// #include "SwerveTranslation2d.h"

// SwerveTranslation2d::SwerveTranslation2d() {
// 	m_x = 0;
// 	m_y = 0;
// }

// SwerveTranslation2d::SwerveTranslation2d(double x, double y) {
// 	m_x = x;
// 	m_y = y;
// }

// double SwerveTranslation2d::Norm() {
// 	return hypot(m_x, m_y);
// }

// double SwerveTranslation2d::GetX() {
// 	return m_x;
// }

// double SwerveTranslation2d::GetY() {
// 	return m_y;
// }

// void SwerveTranslation2d::SetX(double x) {
// 	m_x = x;
// }

// void SwerveTranslation2d::SetY(double y) {
// 	m_y = y;
// }

// SwerveTranslation2d SwerveTranslation2d::TranslateBy(SwerveTranslation2d other) {
// 	return SwerveTranslation2d(m_x + other.GetX(), m_y + other.GetY());
// }

// SwerveTranslation2d SwerveTranslation2d::RotateBy(SwerveRotation2d rotation) {
// 	return SwerveTranslation2d(m_x * rotation.GetCos() - m_y * rotation.GetSin(),
// 			m_x * rotation.GetSin() + m_y * rotation.GetCos());
// }

// SwerveTranslation2d SwerveTranslation2d::Inverse() {
// 	return SwerveTranslation2d(-m_x, -m_y);
// }

// SwerveTranslation2d SwerveTranslation2d::Interpolate(SwerveTranslation2d other, double x) {
// 	if(x <=0){
// 		return *this;
// 	} else if (x >= 1){
// 		return other;
// 	}
// 	return Extrapolate(other, x);
// }

// SwerveTranslation2d SwerveTranslation2d::Extrapolate(SwerveTranslation2d other, double x) {
// 	return SwerveTranslation2d(x * (other.GetX() - m_x) + m_x, x * (other.GetY() - m_y) + m_y);
// }

// SwerveTranslation2d SwerveTranslation2d::FlipX() {
// 	return SwerveTranslation2d(-m_x, m_y);
// }

// SwerveTranslation2d SwerveTranslation2d::FlipY() {
// 	return SwerveTranslation2d(m_x, -m_y);
// }
// double SwerveTranslation2d::GetSin() {
//     return GetY() / Norm();
// }
// double SwerveTranslation2d::GetCos() {
//     return GetX() / Norm();
// }