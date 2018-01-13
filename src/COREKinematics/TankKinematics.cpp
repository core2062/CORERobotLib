#include "TankKinematics.h"

double TankKinematics::wheelDiameter;
double TankKinematics::scrubFactor;

VelocityPair::VelocityPair(double l, double r) {
    left = l;
    right = r;
}

Position2d::Delta TankKinematics::forwardKinematics(double leftDelta,
                                                    double rightDelta) {
    wheelDiameter = 3.949;
    scrubFactor = 0.04;    
    double linearVel = (leftDelta + rightDelta) / 2.0;
    double deltaV = (rightDelta - leftDelta) / 2.0;
    double deltaRot = deltaV * 2 * scrubFactor / wheelDiameter;
    return Position2d::Delta(linearVel, 0, deltaRot);
}

Position2d::Delta TankKinematics::forwardKinematics(double leftDelta,
                                                    double rightDelta, double deltaRads) {
    return Position2d::Delta((leftDelta + rightDelta) / 2.0, 0, deltaRads);
}

Position2d TankKinematics::integrateForwardKinematics(Position2d pos,
                                                      double leftDelta, double rightDelta, COREVector heading) {
    Position2d::Delta withGyro = forwardKinematics(leftDelta, rightDelta,
                                                   pos.getRotation().RotationInverse().RotateBy(heading).GetRadians());
    return pos.transformBy(Position2d::fromVelocity(withGyro));
}

VelocityPair TankKinematics::inverseKinematics(Position2d::Delta vel) {
    if (fabs(vel.dtheta) < kE) {
        return VelocityPair(vel.dx, vel.dx);
    }
    double deltaV = wheelDiameter * vel.dtheta / (2 * scrubFactor);
    return VelocityPair(vel.dx + deltaV, vel.dx - deltaV);
}
