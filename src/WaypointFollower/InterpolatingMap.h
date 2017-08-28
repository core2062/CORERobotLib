#pragma once

#include "COREUtilities/Position2d.h"
#include <map>

struct InterpolatingDouble{
	double value = 0.0;

	InterpolatingDouble(double val);

	InterpolatingDouble interpolate(InterpolatingDouble other, double x);

	double inverseInterpolate(InterpolatingDouble upper, InterpolatingDouble query);

	bool operator<(const InterpolatingDouble& other) const;
};

class InterpolatingTreeMap{
private:
	std::map<InterpolatingDouble, Position2d> m_values;
	unsigned int m_max;

public:
	InterpolatingTreeMap(int maxSize = 0);

	Position2d put(InterpolatingDouble key, Position2d value);

	Position2d getInterpolated(InterpolatingDouble key);

	Position2d getLatest();
};
