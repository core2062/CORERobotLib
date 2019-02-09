#include "InterpolatingMap.h"

InterpolatingDouble::InterpolatingDouble(double val) {
	value = val;
}

InterpolatingDouble InterpolatingDouble::Interpolate(InterpolatingDouble other,
		double x) {
	double dydx = other.value - value;
	double searchY = dydx * x + value;
	return InterpolatingDouble(searchY);
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
	Put(InterpolatingDouble(0.0), Position2d());
}

Position2d InterpolatingTreeMap::Put(InterpolatingDouble key,
		Position2d value) {
	if (m_max > 0 && m_max <= m_values.size()){
		m_values.erase(m_values.begin());
	}
	m_values[key] = value;
	return value;
}

Position2d InterpolatingTreeMap::GetInterpolated(InterpolatingDouble key) {
	if (m_values.count(key)){
		return m_values[key];
	} else {
		auto topBound = m_values.upper_bound(key);
		auto botBound = m_values.lower_bound(key);

		bool top = (topBound == m_values.end());
		bool bot = (botBound == m_values.end());
		if(top && bot){
			return Position2d(Translation2d(),Rotation2d());
		}else if (top){
			return botBound->second;
		}else if (bot){
			return topBound->second;
		}

		Position2d topElem = topBound->second;
		Position2d botElem = botBound->second;
		InterpolatingDouble b = botBound->first;
		InterpolatingDouble t = topBound->first;
		return botElem.Interpolate(topElem, b.InverseInterpolate(t, key));
	}
}

Position2d InterpolatingTreeMap::GetLatest() {
	return (*m_values.rbegin()).second;
}
