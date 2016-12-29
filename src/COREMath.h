#pragma once

#include <cmath>

namespace CORE{

	class Vector{
	public:
		double x = 0.0;
		double y = 0.0;

		Vector(double newX = 0.0, double newY = 0.0) {
			x = newX;
			y = newY;
		}
		void operator= (double val[2]) {
			x = val[0];
			y = val[1];
		}
		double length() {
			return std::sqrt(std::pow(x,2) + std::pow(y,2));
		}
		double angle() {
			return (std::atan2(x,y)*180)/3.1415;
		}
		Vector unit() {
			Vector v;
			v.x = x/length();
			v.y = y/length();
			return v;
		}
	};
}
