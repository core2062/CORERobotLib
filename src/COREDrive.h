#pragma once

#include <vector>
#include <cmath>

#include "COREHardware.h"
#include "COREHardware/CORESensor.h"

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
		class SwerveModule : public COREContinuous {
        public:
			SwerveModule(shared_ptr<COREMotor> driveMotor, shared_ptr<COREMotor> steerMotor,
                         shared_ptr<COREEncoder> steerEncoder) {
				drive = driveMotor;
				rotation = steerMotor;
				encoder = steerEncoder;
			}
			double getCurrentAngle() {
				return clamp(encoder->Get() * encoderRatio);
			}
			double getSetValue(double angle) {
				int direction = encoder->Get()/abs(encoder->Get());
				double base = (abs(encoder->Get()) - (int)(abs(encoder->Get())) % (int)(360/encoderRatio));
				return base * direction + angle/encoderRatio;
			}
			shared_ptr<COREMotor> drive;
			shared_ptr<COREMotor> rotation;
			shared_ptr<COREEncoder> encoder;
			double setMagnitude = 0;
			double setAngle = 0;
			int setDirection = 1;
			Vector position;
			double encoderRatio = (360/1024.0) * (1/(16*4.22));
		};

		enum ControlMode{
			MECANUM,
			MECANUMTHROTTLE,
			FIELDMECANUM,
			FIELDMECANUMTHROTTLE,

			SMARTMECANUM,
			SMARTFIELDMECANUM,

			TEST

		};

		SwerveDrive(vector<shared_ptr<SwerveModule>> swerves);

		void robotInit();
		void setMode(ControlMode m);
		void releaseMode();
		void run(double y, double x, double r);

    protected:

		ControlMode m_mode = MECANUM;
		bool m_forceMode = false;

		double m_wheelbase = 0.0;
		double m_trackwidth = 0.0;

		JoystickAxis m_xAxis = JoystickAxis::LEFT_STICK_X;
		JoystickAxis m_yAxis = JoystickAxis::LEFT_STICK_Y;
		JoystickAxis m_rotXAxis = JoystickAxis::RIGHT_STICK_X;
		JoystickAxis m_rotYAxis = JoystickAxis::RIGHT_STICK_Y;
		JoystickAxis m_throttleAxis = JoystickAxis::LEFT_TRIGGER_AXIS;

		double m_rotationScalar = 1.0;

		double m_y = 0.0;
		double m_x = 0.0;
		double m_rot = 0.0;
		double m_throttle = 0.0;

    private:
		SendableChooser m_chooser;

		vector<shared_ptr<SwerveModule>> m_modules;

		void checkSD();

		void setMotors();

		void calculateNormal();
		void calculateSmart();

		void doMecanum();
		void doMecanumThrottle();
		void doFieldMecanum();
		void doFieldMecanumThrottle();
		void doSmartMecanum();
		void doSmartFieldMecanum();
		void doTest();

    };

	class AetherDrive {
	private:

	public:
		AetherDrive();

	};
}
