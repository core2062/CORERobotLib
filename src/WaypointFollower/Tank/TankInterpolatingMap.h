#pragma once

#include "TankPosition2d.h"
#include <map>

struct TankInterpolatingDouble{
	double value = 0.0;

	TankInterpolatingDouble(double val);

	TankInterpolatingDouble Interpolate(TankInterpolatingDouble other, double x);

	double InverseInterpolate(TankInterpolatingDouble upper, TankInterpolatingDouble query);

	bool operator<(const TankInterpolatingDouble& other) const;
};

class TankInterpolatingTreeMap{
private:
	std::map<TankInterpolatingDouble, TankPosition2d> m_values;
	unsigned int m_max;

public:
	TankInterpolatingTreeMap(int maxSize = 0);

	TankPosition2d Put(TankInterpolatingDouble key, TankPosition2d value);

	TankPosition2d GetInterpolated(TankInterpolatingDouble key);

	TankPosition2d GetLatest();
};