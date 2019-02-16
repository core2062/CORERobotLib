#pragma once

#include "SwervePosition2d.h"
#include <map>

struct SwerveInterpolatingDouble{
	double value = 0.0;

	SwerveInterpolatingDouble(double val);

	SwerveInterpolatingDouble Interpolate(SwerveInterpolatingDouble other, double x);

	double InverseInterpolate(SwerveInterpolatingDouble upper, SwerveInterpolatingDouble query);

	bool operator<(const SwerveInterpolatingDouble& other) const;
};

class SwerveInterpolatingTreeMap{
private:
	std::map<SwerveInterpolatingDouble, SwervePosition2d> m_values;
	unsigned int m_max;

public:
	SwerveInterpolatingTreeMap(int maxSize = 0);

	SwervePosition2d Put(SwerveInterpolatingDouble key, SwervePosition2d value);

	SwervePosition2d GetInterpolated(SwerveInterpolatingDouble key);

	SwervePosition2d GetLatest();

};
