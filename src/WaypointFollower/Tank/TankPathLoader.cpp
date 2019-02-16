#include "TankInterpolatingMap.h"

TankInterpolatingDouble::TankInterpolatingDouble(double val) {
	value = val;
}

TankInterpolatingDouble InterpolatingDouble::Interpolate(TankInterpolatingDouble other,
		double x) {
	double dydx = other.value - value;
	double searchY = dydx * x + value;
	return TankInterpolatingDouble(searchY);
}

double InterpolatingDouble::InverseInterpolate(InterpolatingDouble upper,
		InterpolatingDouble query) {
	double upperToLower = upper.value - value;
	if (upperToLower <= 0){
		return 0;
	}
	double queryToLower = query.value - value;
	if (queryToLower <= 0){
		return 0;
	}
	return queryToLower / upperToLower;
}

bool InterpolatingDouble::operator<(const InterpolatingDouble& other) const {
	return (value < other.value);
}

InterpolatingTreeMap::InterpolatingTreeMap(int maxSize) {
	m_max = maxSize;
	put(InterpolatingDouble(0.0), Position2d());
}

Position2d InterpolatingTreeMap::put(InterpolatingDouble key,
		Position2d value) {
	if (m_max > 0 && m_max <= m_values.size()){
		m_values.erase(m_values.begin());
	}
	m_values[key] = value;
	return value;
}

TankPosition2d TankInterpolatingTreeMap::GetInterpolated(TankInterpolatingDouble key) {
	if (m_values.count(key)){
		return m_values[key];
	} else {
		auto topBound = m_values.upper_bound(key);
		auto botBound = m_values.lower_bound(key);

		bool top = (topBound == m_values.end());
		bool bot = (botBound == m_values.end());
		if(top && bot){
			return TankPosition2d(TankTranslation2d(), TankRotation2d());
		}else if (top){
			return botBound->second;
		}else if (bot){
			return topBound->second;
		}

		TankPosition2d topElem = topBound->second;
		TankPosition2d botElem = botBound->second;
		TankInterpolatingDouble b = botBound->first;
		TankInterpolatingDouble t = topBound->first;
		return botElem.Interpolate(topElem, b.InverseInterpolate(t, key));
	}
}

TankPosition2d TankInterpolatingTreeMap::GetLatest() {
	return (*m_values.rbegin()).second;
}
