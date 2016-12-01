#pragma once

#include <iostream>
#include <memory>
#include "COREMotor.h"

using namespace std;

namespace CORE {
	enum distanceUnit {
		FEET,
		INCHES,
		CENTIMETERS,
		METERS
	};
	class COREEncoder {
	public:
		COREEncoder(int port, bool reversed = false);
		COREEncoder(shared_ptr<COREMotor> motor, bool reversed = false);
		void setReversed(bool reversed = true);
		bool getReversed();
		double Get();
		double getDistance(distanceUnit unit = FEET);
		void setDistance(distanceUnit unit = FEET);
		void Set();
		void Reset();
	private:
		double m_ticksToDistanceConversion[4];
	};
}