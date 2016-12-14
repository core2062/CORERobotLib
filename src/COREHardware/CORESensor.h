#pragma once

namespace CORE{

	class CORESensor{
	public:
		virtual double Get() = 0;
		virtual ~CORESensor();
	};

	class COREContinuous{
	private:
		double m_minVal = 0;
		double m_maxVal = 360;
	public:
		void setMin(double min);
		void setMax(double max);
		double clamp(double input);
	};
}
