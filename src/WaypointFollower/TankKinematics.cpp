#include "TankKinematics.h"

CORE::COREConstant<double> TankKinematics::wheelDiameter("Wheel Diameter", 3.949);
CORE::COREConstant<double> TankKinematics::scrubFactor("Scrub Factor", .04);

VelocityPair::VelocityPair(double l, double r) {
	left = l;
	right = r;
}

Position2d::Delta TankKinematics::ForwardKinematics(double leftDelta,
		double rightDelta) {
	double linearVel = (leftDelta + rightDelta) / 2.0;
	double deltaV = (rightDelta - leftDelta) / 2.0;
	double deltaRot = deltaV * 2 * scrubFactor.Get() / wheelDiameter.Get();
	return Position2d::Delta(linearVel, 0, deltaRot);
}

Position2d::Delta TankKinematics::ForwardKinematics(double leftDelta,
		double rightDelta, double deltaRads) {
	return Position2d::Delta((leftDelta + rightDelta) / 2.0, 0, deltaRads);
}

Position2d TankKinematics::IntegrateForwardKinematics(Position2d pos,
		double leftDelta, double rightDelta, Rotation2d heading) {
	Position2d::Delta withGyro = ForwardKinematics(leftDelta, rightDelta,
			pos.GetRotation().Inverse().RotateBy(heading).GetRadians());
	return pos.TransformBy(Position2d::FromVelocity(withGyro));
}

VelocityPair TankKinematics::InverseKinematics(Position2d::Delta vel) {
	if(fabs(vel.dtheta) < kE){
		return VelocityPair(vel.dx, vel.dx);
	}
	double deltaV = wheelDiameter.Get() * vel.dtheta / (2 * scrubFactor.Get());
	return VelocityPair(vel.dx + deltaV, vel.dx - deltaV);
}
