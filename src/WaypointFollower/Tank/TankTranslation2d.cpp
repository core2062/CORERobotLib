#include "TankTranslation2d.h"
#include <COREUtilities/COREMath.h>

TankTranslation2d::TankTranslation2d() {
	m_x = 0;
	m_y = 0;
}

TankTranslation2d::TankTranslation2d(double x, double y) {
	cout<<"TankTraslation2d::TankTraslation2d 3"<<endl;
	m_x = x;
	m_y = y;
}

double TankTranslation2d::Norm() {
	return CORE::Pathogram(m_x, m_y);
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
	cout<<"TankTraslation2d::TranslateBy"<<endl;
	return TankTranslation2d(m_x + other.GetX(), m_y + other.GetY());
}

TankTranslation2d TankTranslation2d::RotateBy(TankRotation2d rotation) {
	cout<<"TankTranslation2d::RotateBy"<<endl;
	return TankTranslation2d(m_x * rotation.GetCos() - m_y * rotation.GetSin(),
			m_x * rotation.GetSin() + m_y * rotation.GetCos());
}

TankTranslation2d TankTranslation2d::Inverse() {
	cout<<"TankTranslation::Inverse"<<endl;
	return TankTranslation2d(-m_x, -m_y);
}

TankTranslation2d TankTranslation2d::Interpolate(TankTranslation2d other, double x) {
	cout<<"TankTranslation2d::Interpolate"<<endl;
	if(x <=0){
		return *this;
	} else if (x >= 1){
		return other;
	}
	return Extrapolate(other, x);
}

TankTranslation2d TankTranslation2d::Extrapolate(TankTranslation2d other, double x) {
	cout<<"TankTranslation2d::Extrapolate"<<endl;
	return TankTranslation2d(x * (other.GetX() - m_x) + m_x, x * (other.GetY() - m_y) + m_y);
}

TankTranslation2d TankTranslation2d::FlipX() {
	cout<<"TankTranslation2d::FlipX"<<endl;
	return TankTranslation2d(-m_x, m_y);
}

TankTranslation2d TankTranslation2d::FlipY() {
	cout<<"TankTranslation2d::FlipY"<<endl;
	return TankTranslation2d(m_x, -m_y);
}
