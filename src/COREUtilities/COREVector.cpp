#include "COREVector.h"


using namespace CORE;

COREVector::COREVector() {
    m_theta = 0;
    m_r = 0;
}

COREVector::COREVector(const COREVector &other) {
    m_r = other.m_r;
    m_theta = other.m_theta;
}

COREVector::COREVector(double r, double theta) {
    m_r = r;
    m_theta = theta;
}

COREVector COREVector::FromRadians(double radians, double mag) {
    return COREVector(mag, radians);
}

COREVector COREVector::FromDegrees(double degrees, double mag) {
    return FromRadians(toRadians(degrees), mag);
}

COREVector COREVector::FromCompassDegrees(double compassDegrees, double mag) {
    double degrees = 90 - compassDegrees;
    degrees = degrees < 0 ? 360 + degrees : degrees;
    return FromRadians(toRadians(degrees), mag);
}

COREVector COREVector::FromXY(double x, double y) {
    return COREVector(hypot(x, y), atan2(y, x));
}

void COREVector::Normalize() {
    if(m_r < 0) {
        Opposite();
    }
    m_r = 1;
}

double COREVector::NormalizeMagnitude() {
    m_r = 1;
    return 0;
    //TODO: figure out what this is supposed to do and fix this
}

double COREVector::GetRadians() {
    return m_theta;
}

double COREVector::GetDegrees() {
    return CORE::toDegrees(GetRadians());
}

double COREVector::GetCompassDegrees() {
    double degrees = 450 - GetDegrees();
    return (degrees >= 360 ? degrees - 360 : degrees);
}

COREVector COREVector::RotateBy(COREVector other) {
    return COREVector(m_r, m_theta + other.m_theta);
}

COREVector COREVector::RotationInverse() {
    return COREVector(m_r, fmod(m_theta + PI, 2 * PI));
}

COREVector COREVector::Opposite() {
    return COREVector(-m_r, fmod(m_theta + PI, 2 * PI));
}

COREVector COREVector::InterpolateRotation(COREVector other, double x) {
    if (x <= 0) {
        return *this;
    } else if (x >= 1) {
        return other;
    }
    double diff = RotationInverse().RotateBy(other).GetRadians();
    return RotateBy(FromRadians(diff * x));
}

double COREVector::GetX() {
    return cos(m_theta) * m_r;
}

double COREVector::GetY() {
    return sin(m_theta) * m_r;
}

void COREVector::Invert() {
    m_r *= -1;
    m_theta = fmod(m_theta + PI, 2 * PI);
}

double COREVector::GetMagnitude() {
    return m_r;
}

void COREVector::SetXY(double x, double y) {
    m_r = hypot(x, y);
    m_theta = atan2(y, x);
}

COREVector COREVector::TranslateBy(COREVector other) {
    return COREVector(m_r + other.m_r, m_theta + other.m_theta);
}

COREVector COREVector::MagnitudeInverse() {
    return COREVector(-m_r, m_theta);
}

COREVector COREVector::InterpolateMagnitude(COREVector other, double x) {
    if (x <= 0) {
        return *this;
    } else if (x >= 1) {
        return other;
    }
    return Extrapolate(other, x);
}

COREVector COREVector::Extrapolate(COREVector other, double x) {
    return FromXY(x * (other.GetX() - GetX()) + GetX(), x * (other.GetY() - GetY()) + GetY());
}

COREVector COREVector::FlipX() {
    //return COREVector(-m_x, m_y);
    return COREVector(0, 0);
}

COREVector COREVector::FlipY() {
    //return COREVector(m_x, -m_y);
    return COREVector(0, 0);
}

COREVector COREVector::AddVector(COREVector firstVector) {
    return FromXY(GetX() + firstVector.GetX(), GetY() + firstVector.GetY());
}

COREVector COREVector::SubtractVector(COREVector firstVector) {
    return FromXY(GetX() - firstVector.GetX(), GetY() - firstVector.GetY());
}

double COREVector::GetDotProduct(COREVector firstVector) {
    return GetX() * firstVector.GetX() + GetY() + firstVector.GetY();
}

double COREVector::GetCrossProduct(COREVector firstVector) {
    return GetX() * firstVector.GetY() - GetY() * firstVector.GetX();
}

double COREVector::ShortestRotationTo(COREVector target) {
    double counterClockwiseMove = GetCompassDegrees() - target.GetCompassDegrees();
    double clockwiseMove = target.GetCompassDegrees() - GetCompassDegrees();
    clockwiseMove += (clockwiseMove < 0 ? 360 : 0);
    counterClockwiseMove += (counterClockwiseMove < 0 ? 360 : 0);
    return (abs(clockwiseMove) < abs(counterClockwiseMove) ? clockwiseMove : -counterClockwiseMove);
}

void COREVector::SetMagnitude(double magnitude) {
    m_r = magnitude;
}
double COREVector::GetCos() {
    return cos(m_theta);
}
double COREVector::GetSin() {
    return sin(m_theta);
}
