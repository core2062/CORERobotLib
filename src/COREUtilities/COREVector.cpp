#include "COREVector.h"


using namespace CORE;

COREVector::COREVector() {
  m_x = 0;
  m_y = 0;
}

COREVector::COREVector(double x, double y, bool doNormalize) {
  m_cos = x;
  m_sin = y;
  if (doNormalize) {
    NormalizeRotation();
    NormalizeMagnitude();
  }
}

COREVector::COREVector(const COREVector& other) {
  m_cos = other.m_cos;
  m_sin = other.m_sin;
}

COREVector::COREVector(double x, double y) {
  m_x = x;
  m_y = y;
}

COREVector COREVector::FromRadians(double radians) {
  return COREVector(cos(radians), sin(radians), false);
}

COREVector COREVector::FromDegrees(double degrees) {
  return FromRadians(toRadians(degrees));
}

COREVector COREVector::FromCompassDegrees(double compassDegrees) {
  double degrees = 90 - compassDegrees;
  degrees = degrees < 0 ? 360 + degrees : degrees;
  return FromRadians(toRadians(degrees));
}

void COREVector::NormalizeRotation() {
  double mag = pathogram(m_cos, m_sin);
  if (mag > kE){
    m_sin /= mag;
    m_cos /= mag;
  } else {
    m_sin = 0;
    m_cos = 1;
  }
}

double COREVector::GetCos() {
  return m_cos;
}

double COREVector::GetSin() {
  return m_sin;
}

double COREVector::GetRadians() {
  return atan2(m_sin, m_cos);
}

double COREVector::GetDegrees() {
  return CORE::toDegrees(GetRadians());
}

double COREVector::GetCompassDegrees() {
    double degrees = 450 - GetDegrees();
    return degrees >= 360 ? degrees - 360 : degrees;
}

COREVector COREVector::RotateBy(COREVector other) {
  return COREVector(m_cos * other.GetCos() - m_sin * other.GetSin(),
      m_cos * other.GetSin() + m_sin * other.GetCos(), true);
}

COREVector COREVector::RotationInverse() {
  return COREVector(m_cos, -m_sin, false);
}

COREVector COREVector::Opposite() {
  return COREVector(-m_cos, -m_sin, false);
}

COREVector COREVector::InterpolateRotation(COREVector other, double x) {
  if (x <= 0){
    return *this;
  } else if (x >= 1){
    return other;
  }
  double diff = RotationInverse().RotateBy(other).GetRadians();
  return RotateBy(FromRadians(diff * x));
}

double COREVector::NormalizeMagnitude() {
  return CORE::pathogram(m_x, m_y);
}

double COREVector::GetX() {
  return m_x;
}

double COREVector::GetY() {
  return m_y;
}

double COREVector::GetMagnitude() {
  return NormalizeMagnitude();
}

void COREVector::SetX(double x) {
  m_x = x;
}

void COREVector::SetY(double y) {
  m_y = y;
}

COREVector COREVector::TranslateBy(COREVector other) {
  return COREVector(m_x + other.GetX(), m_y + other.GetY());
}

COREVector COREVector::MagnitudeInverse() {
  return COREVector(-m_x, -m_y);
}

COREVector COREVector::InterpolateMagnitude(COREVector other, double x) {
  if(x <=0){
    return *this;
  } else if (x >= 1){
    return other;
  }
  return Extrapolate(other, x);
}

COREVector COREVector::Extrapolate(COREVector other, double x) {
  return COREVector(x * (other.GetX() - m_x) + m_x, x * (other.GetY() - m_y) + m_y);
}

COREVector COREVector::FlipX() {
  return COREVector(-m_x, m_y);
}

COREVector COREVector::FlipY() {
  return COREVector(m_x, -m_y);
}

COREVector COREVector::AddVector(COREVector firstVector) {
  return COREVector(GetX() + firstVector.GetX(), GetY() + firstVector.GetY());
}

COREVector COREVector::SubtractVector(COREVector firstVector) {
  return COREVector(GetX() - firstVector.GetX(), GetY() - firstVector.GetY());
}

double COREVector::GetDotProduct(COREVector firstVector) {
  return GetX() * firstVector.GetX() + GetY() + firstVector.GetY();
}

double COREVector::GetCrossProduct(COREVector firstVector) {
  return GetX() * firstVector.GetY() - GetY() * firstVector.GetX();
}

