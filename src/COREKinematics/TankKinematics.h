#pragma once

#include "COREUtilities/Position2d.h"
#include "COREUtilities/COREConstant.h"

struct VelocityPair {
    double left;
    double right;

    VelocityPair(double l, double r);
};

class TankKinematics {
public:
    static Position2d::Delta forwardKinematics(double leftDelta, double rightDelta);

    static Position2d::Delta forwardKinematics(double leftDelta, double rightDelta, double deltaRads);

    static Position2d
    integrateForwardKinematics(Position2d pos, double leftDelta, double rightDelta, COREVector heading);

    static VelocityPair inverseKinematics(Position2d::Delta vel);

    static double wheelDiameter; //TODO: Change this so that thease can be changed
    static double scrubFactor;
};