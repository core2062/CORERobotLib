#pragma once

#include <iostream>

#include "../CORETask.h"
#include "../COREHardware/COREMotor.h"
#include "../COREConstant.h"
#include "../COREDrive.h"

namespace CORE {
    class COREEtherDrive : public COREDrive {
    public:
        COREEtherDrive(COREMotor* leftMotor1, COREMotor* leftMotor2, COREMotor* rightMotor1, COREMotor* rightMotor2,
                       double a, double b, double quickTurn);
    protected:
        void update() override;
    private:
        COREMotor* m_leftMotor1, * m_leftMotor2, * m_rightMotor1, * m_rightMotor2;
        double etherL(double fwd, double rcw, double a, double b);
        double etherR(double fwd, double rcw, double a, double b);
        double m_a, m_b, m_quickTurn, m_left, m_right;
    };
}
