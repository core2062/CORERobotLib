#pragma once

#include <iostream>
#include <memory>
//#include "COREMotor.h"

#include "WPILib.h"
#include "CANTalon.h"

using namespace std;

namespace CORE {
	enum distanceUnit {
		FEET,
		INCHES,
		CENTIMETERS,
		METERS
	};

	enum encoderType {
		QUAD,
		SRX_RELATIVE,
		SRX_ABSOLUTE,
		ANALOG_POT
	};
	class COREEncoder {
	public:
		shared_ptr<Encoder> encoder;
        COREEncoder(shared_ptr<CANTalon> CANTalonController, encoderType encoderType);
        COREEncoder(int portA, int portB, encoderType encoder, bool reversed = false);
//        COREEncoder(shared_ptr<COREMotor> motor, encoderType encoder);
		void setReversed(bool reversed = true);
		bool getReversed();
		int Get();
		double getDistance(distanceUnit unit = FEET);
		void setDistance(distanceUnit unit = FEET);
		void Set(int value);
		void Reset();
    protected:
        bool m_bondToCANTalon = false;
        shared_ptr<CANTalon> m_CANTalonController;
		double m_ticksToDistanceConversion[4];
//		shared_ptr<COREMotor> m_motor;
		encoderType m_encoderType;
		int m_port;
		bool m_reversed;
		int m_offset;
	};
}
