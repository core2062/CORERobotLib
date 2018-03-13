#pragma once

#include "COREUtilities/COREMath.h"
#include <cmath>

class COREVector {
public:
	COREVector();
	COREVector(double r, double theta);
	COREVector(const COREVector& other);
    double GetCompassDegrees();
  	double GetX();
  	double GetY();
	static COREVector FromCompassDegrees(double compassDegrees, double mag = 1);
	static COREVector FromRadians(double radians, double mag = 1);
	static COREVector FromDegrees(double degrees, double mag = 1);
	static COREVector FromXY(double x, double y);
	COREVector RotationInverse();
	COREVector MagnitudeInverse();
	COREVector Opposite();
	COREVector FlipX();
	COREVector FlipY();
	COREVector RotateBy(COREVector rotation);
	COREVector InterpolateRotation(COREVector other, double x);
	COREVector InterpolateMagnitude(COREVector other, double x);
	COREVector TranslateBy(COREVector other);
	COREVector Extrapolate(COREVector other, double x);
	double ShortestRotationTo(COREVector target);
	void Normalize();
    double NormalizeMagnitude();
	void SetXY(double x, double y);
	void SetMagnitude(double magnitude);
    void Invert();
  	COREVector AddVector(COREVector firstVector);
	COREVector SubtractVector(COREVector firstVector);
  	double GetCrossProduct(COREVector firstVector);
  	double GetDotProduct(COREVector firstVector);
  	double GetMagnitude();
  	double GetDegrees();
  	double GetRadians();
  	double GetCos();
  	double GetSin();

protected:
  	double m_theta = 0;
    double m_r = 0;
};
