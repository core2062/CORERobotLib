#pragma once

#include <iostream>
#include "COREFramework/CORETask.h"
#include "COREUtilities/COREConstant.h"
#include "COREDrive.h"
#include "WaypointFollower/TankKinematics.h"

namespace CORE {
	class COREEtherDrive: public COREDrive {
	public:
		COREEtherDrive(TalonSRX* leftMotor1, TalonSRX* leftMotor2,
				TalonSRX* rightMotor1, TalonSRX* rightMotor2, double a, double b,
				double quickTurn);
		static void SetAB(double a, double b);
		static void SetQuickturn(double q);
		static VelocityPair Calculate(double mag, double rot, double deadband = .1);
	protected:
		void Update() override;
	private:
		TalonSRX * m_leftMotor1, *m_leftMotor2, *m_rightMotor1, *m_rightMotor2;
		double static EtherL(double fwd, double rcw, double a, double b);
		double static EtherR(double fwd, double rcw, double a, double b);
		double m_left = 0.0, m_right = 0.0;
		static double m_a, m_b, m_quickTurn;
	};
}
