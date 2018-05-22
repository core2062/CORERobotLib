#pragma once

#include "Position2d.h"
#include <map>

struct InterpolatingDouble{
	double value = 0.0;

	InterpolatingDouble(double val);

	InterpolatingDouble Interpolate(InterpolatingDouble other, double x);

	double InverseInterpolate(InterpolatingDouble upper, InterpolatingDouble query);

	bool operator<(const InterpolatingDouble& other) const;
};

class InterpolatingTreeMap{
private:
	std::map<InterpolatingDouble, Position2d> m_values;
	unsigned int m_max;

public:
	InterpolatingTreeMap(int maxSize = 0);

	Position2d Put(InterpolatingDouble key, Position2d value);

	Position2d GetInterpolated(InterpolatingDouble key);

	Position2d GetLatest();

};
