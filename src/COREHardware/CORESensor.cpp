#include "CORESensor.h"

using namespace CORE;

void COREContinuous::setMin(double min){
	m_minVal = min;
}

void COREContinuous::setMax(double max){
	m_maxVal = max;
}

double COREContinuous::clamp(double input){
	double output = input;
	while (output > m_maxVal){
		output -= m_maxVal - m_minVal;
	}
	while (output < m_minVal){
		output += m_maxVal - m_minVal;
	}
	return output;
}
