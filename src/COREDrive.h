#pragma once

#include <vector>

#include "COREHardware.h"

namespace CORE {
	class TankDrive {
	public:
		//TankDrive(COREMotor * frontLeft, COREMotor * backLeft, COREMotor * frontRight, COREMotor * backRight);
		void SetLeftSpeed();
		void SetRightSpeed();
		void SetSpeed();

	};

	class MecanumDrive {
	private:

	public:
		MecanumDrive();
	};

	class SwerveDrive {
	private:

	public:
		SwerveDrive();
	};

	class AetherDrive {
	private:

	public:
		AetherDrive();

	};
}