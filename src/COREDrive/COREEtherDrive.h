#pragma once

#include <iostream>

#include <CORETask.h>
#include <COREHardware/COREMotor.h>
#include <COREConstant.h>
#include <COREDrive.h>
#include "WaypointFollower/TankKinematics.h"

namespace CORE {
    class COREEtherDrive : public COREDrive {
    public:
        COREEtherDrive(COREMotor * leftMotor1, COREMotor * leftMotor2, COREMotor * rightMotor1, COREMotor * rightMotor2,
                       double a, double b, double quickTurn);
        static void setAB(double a, double b);
        static void setQuickturn(double q);
        static VelocityPair calculate(double mag, double rot, double deadband = .1);
    protected:
        void update() override;
    private:
        COREMotor * m_leftMotor1, * m_leftMotor2, * m_rightMotor1, * m_rightMotor2;
        double static etherL(double fwd, double rcw, double a, double b);
        double static etherR(double fwd, double rcw, double a, double b);
        double m_left = 0.0, m_right = 0.0;
        static double m_a, m_b, m_quickTurn;
    };
}
