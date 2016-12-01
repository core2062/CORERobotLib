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
	public:
		SwerveDrive(shared_ptr<COREMotor> frontLeftDrive, shared_ptr<COREMotor> frontLeftSteer,
                    shared_ptr<COREMotor> backLeftDrive, shared_ptr<COREMotor> backLeftSteer,
                    shared_ptr<COREMotor> frontRightDrive, shared_ptr<COREMotor> frontRightSteer,
                    shared_ptr<COREMotor> backRightDrive, shared_ptr<COREMotor> backRightSteer);
    private:
    };

	class AetherDrive {
	private:

	public:
		AetherDrive();

	};
}