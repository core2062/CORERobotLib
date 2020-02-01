#pragma once

#include <cmath>
#include <iostream>

const double PI = 3.141592653589793;
#define kE .000000001

using namespace std;

namespace CORE {

    inline double ToRadians(double degrees) {
        return degrees * (PI / 180.0);
    }

    inline double ToDegrees(double degrees) {
        return degrees * (180.0 / PI);
    }

    inline double Arctan(double x, double y) {
        double result = ToDegrees(atan2(y, x));
        if(x >= 0 && y >= 0) {
            return 90 - result;
        } else if(x >= 0 && y < 0) {
            return 90 + -1 * result;
        } else if(x <= 0 && y < 0) {
            return 90 + -1 * result;
        } else if(x < 0 && y >= 0) {
            return 450 - result;
        }
        return -1;
    }

	inline double Pathogram(double x, double y){
		return sqrt(x*x + y*y);
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
		double Length() {
			return sqrt(pow(x,2) + pow(y,2));
		}
		double Angle() {
			return (atan2(x,y)*180)/3.1415;
		}
		Vector Unit() {
			Vector v;
			v.x = x/Length();
			v.y = y/Length();
			return v;
		}
	};
}
