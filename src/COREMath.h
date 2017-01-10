#pragma once

#include <cmath>

#define PI 3.141592653589793


namespace CORE {

	inline double toRadians(double degrees) {
		return degrees * (PI / 180.0);
	}

	inline double toDegrees(double degrees) {
		return degrees * (180.0 / PI);
	}

	inline double arctan(double x, double y) {
		double result = toDegrees(atan2 (y,x));
		if(x >=0 && y >= 0) {
			return 90 - result;
		} else if (x >= 0 && y < 0) {
			return 90 + -1 * result;
		} else if (x <= 0 && y < 0) {
			return 90 + -1 * result;
		} else if (x < 0 && y >= 0) {
			return 450 - result;
		}
		cout << "Oh no" << endl;
		return -1;
	}

	class Vector {
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
