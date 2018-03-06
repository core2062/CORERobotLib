#pragma once

#include "COREUtilities/COREMath.h"

class COREVector {
public:
	COREVector();
	COREVector(double x, double y, bool doNormalize);
	COREVector(const COREVector& other);
	COREVector(double x, double y);
    double GetCompassDegrees();
  	double GetX();
  	double GetY();
	static COREVector FromCompassDegrees(double compassDegrees, double mag = 1);
	static COREVector FromRadians(double radians, double mag = 1);
	static COREVector FromDegrees(double degrees, double mag = 1);
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
	COREVector ShortestRotationTo(COREVector target);
	void NormalizeRotation();
	double NormalizeMagnitude();
	void SetX(double x);
	void SetY(double y);
	void SetMagnitude(double magnitude);
  	COREVector AddVector(COREVector firstVector);
  	COREVector SubtractVector(COREVector firstVector);
  	double GetCrossProduct(COREVector firstVector);
  	double GetDotProduct(COREVector firstVector);
  	double GetMagnitude();
  	double GetDegrees();
  	double GetRadians();
  	double GetCos();
  	double GetSin();

  	double m_direction = 0;
  	double m_magnitude = 0;
protected:
  	double m_sin = 0;
  	double m_cos = 1;
  	double m_x = 0;
  	double m_y = 0;
  	double m_COREVector;
};
