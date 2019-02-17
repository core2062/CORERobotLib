#include "SwerveKinematics.h"

CORE::COREConstant<double> SwerveKinematics::wheelDiameter("Wheel Diameter", 3.949);
CORE::COREConstant<double> SwerveKinematics::scrubFactor("Scrub Factor", .04);

SwerveVelocityPair::SwerveVelocityPair(double l, double r) {
	left = l;
	right = r;
}

SwervePosition2d::SwerveDelta SwerveKinematics::ForwardKinematics(double leftDelta,
		double rightDelta) {
	double linearVel = (leftDelta + rightDelta) / 2.0;
	double deltaV = (rightDelta - leftDelta) / 2.0;
	double deltaRot = deltaV * 2 * scrubFactor.Get() / wheelDiameter.Get();
	return SwervePosition2d::SwerveDelta(linearVel, 0, deltaRot);
}

SwervePosition2d::SwerveDelta SwerveKinematics::ForwardKinematics(double leftDelta,
		double rightDelta, double deltaRads) {
	return SwervePosition2d::SwerveDelta((leftDelta + rightDelta) / 2.0, 0, deltaRads);
}

SwervePosition2d SwerveKinematics::IntegrateForwardKinematics(SwervePosition2d pos,
		double leftDelta, double rightDelta, SwerveRotation2d heading) {
	SwervePosition2d::SwerveDelta withGyro = ForwardKinematics(leftDelta, rightDelta,
			pos.GetRotation().Inverse().RotateBy(heading).GetRadians());
	return pos.TransformBy(SwervePosition2d::FromVelocity(withGyro));
}

SwerveVelocityPair SwerveKinematics::InverseKinematics(SwervePosition2d::SwerveDelta vel) {
	if(fabs(vel.dtheta) < kE){
		return SwerveVelocityPair(vel.dx, vel.dx);
	}
	double deltaV = wheelDiameter.Get() * vel.dtheta / (2 * scrubFactor.Get());
	return SwerveVelocityPair(vel.dx + deltaV, vel.dx - deltaV);
}
