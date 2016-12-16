#pragma once

#include <iostream>
#include <memory>

#include "WPILib.h"

using namespace std;

namespace CORE {
	enum distanceUnit {
		FEET,
		INCHES,
		CENTIMETERS,
		METERS
	};

    enum encoderType {
        NONE,
        QUAD,
        SRX_RELATIVE,
        SRX_ABSOLUTE,
        ANALOG_POT
    };
    class COREMotor;
	class COREEncoder {
	public:
		shared_ptr<Encoder> encoder;
		COREEncoder(int portA, int portB, encoderType encoder, bool reversed);
        COREEncoder(shared_ptr<COREMotor> motor, encoderType encoder);
		void setReversed(bool reversed = true);
		bool getReversed();
		int Get();
        double getDistance(distanceUnit unit = FEET);
        void setDistance(distanceUnit unit = FEET);
        void Set(int value);
        void Reset();
	private:
		double m_ticksToDistanceConversion[4];
        shared_ptr<COREMotor> m_motor = nullptr;
        encoderType m_encoderType;
		int m_port;
		bool m_reversed;
		int m_offset;
	};
}
