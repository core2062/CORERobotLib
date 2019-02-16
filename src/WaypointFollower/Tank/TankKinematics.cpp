#include "TankKinematics.h"

CORE::COREConstant<double> TankKinematics::wheelDiameter("Wheel Diameter", 3.949);
CORE::COREConstant<double> TankKinematics::scrubFactor("Scrub Factor", .04);

VelocityPair::VelocityPair(double l, double r) {
	left = l;
	right = r;
}

TankPosition2d::TankDelta TankKinematics::ForwardKinematics(double leftDelta,
		double rightDelta) {
	double linearVel = (leftDelta + rightDelta) / 2.0;
	double deltaV = (rightDelta - leftDelta) / 2.0;
	double deltaRot = deltaV * 2 * scrubFactor.Get() / wheelDiameter.Get();
	return TankPosition2d::TankDelta(linearVel, 0, deltaRot);
}

TankPosition2d::TankDelta TankKinematics::ForwardKinematics(double leftDelta,
		double rightDelta, double deltaRads) {
	return TankPosition2d::TankDelta((leftDelta + rightDelta) / 2.0, 0, deltaRads);
}

TankPosition2d TankKinematics::IntegrateForwardKinematics(TankPosition2d pos,
		double leftDelta, double rightDelta, TankRotation2d heading) {
	TankPosition2d::TankDelta withGyro = ForwardKinematics(leftDelta, rightDelta,
			pos.GetRotation().Inverse().RotateBy(heading).GetRadians());
	return pos.TransformBy(TankPosition2d::FromVelocity(withGyro));
}

VelocityPair TankKinematics::InverseKinematics(TankPosition2d::TankDelta vel) {
	if(fabs(vel.dtheta) < kE){
		return VelocityPair(vel.dx, vel.dx);
	}
	double deltaV = wheelDiameter.Get() * vel.dtheta / (2 * scrubFactor.Get());
	return VelocityPair(vel.dx + deltaV, vel.dx - deltaV);
}
