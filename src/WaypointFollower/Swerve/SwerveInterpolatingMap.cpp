#include "SwerveInterpolatingMap.h"

SwerveInterpolatingDouble::SwerveInterpolatingDouble(double val) {
	value = val;
}

SwerveInterpolatingDouble SwerveInterpolatingDouble::Interpolate(SwerveInterpolatingDouble other,
		double x) {
	double dydx = other.value - value;
	double searchY = dydx * x + value;
	return SwerveInterpolatingDouble(searchY);
}

double SwerveInterpolatingDouble::InverseInterpolate(SwerveInterpolatingDouble upper,
		SwerveInterpolatingDouble query) {
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

bool SwerveInterpolatingDouble::operator<(const SwerveInterpolatingDouble& other) const {
	return (value < other.value);
}

SwerveInterpolatingTreeMap::SwerveInterpolatingTreeMap(int maxSize) {
	m_max = maxSize;
	Put(SwerveInterpolatingDouble(0.0), SwervePosition2d());
}

SwervePosition2d SwerveInterpolatingTreeMap::Put(SwerveInterpolatingDouble key,
		SwervePosition2d value) {
	if (m_max > 0 && m_max <= m_values.size()){
		m_values.erase(m_values.begin());
	}
	m_values[key] = value;
	return value;
}

SwervePosition2d SwerveInterpolatingTreeMap::GetInterpolated(SwerveInterpolatingDouble key) {
	if (m_values.count(key)){
		return m_values[key];
	} else {
		auto topBound = m_values.upper_bound(key);
		auto botBound = m_values.lower_bound(key);

		bool top = (topBound == m_values.end());
		bool bot = (botBound == m_values.end());
		if(top && bot){
			return SwervePosition2d(SwerveTranslation2d(),SwerveRotation2d());
		}else if (top){
			return botBound->second;
		}else if (bot){
			return topBound->second;
		}

		SwervePosition2d topElem = topBound->second;
		SwervePosition2d botElem = botBound->second;
		SwerveInterpolatingDouble b = botBound->first;
		SwerveInterpolatingDouble t = topBound->first;
		return botElem.Interpolate(topElem, b.InverseInterpolate(t, key));
	}
}

SwervePosition2d SwerveInterpolatingTreeMap::GetLatest() {
	return (*m_values.rbegin()).second;
}
