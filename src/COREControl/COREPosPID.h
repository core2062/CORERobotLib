#pragma once

#include <COREUtilities/CORENamedObject.h>
#include "COREControl/COREController.h"

using namespace CORE;

namespace CORE {
    class COREPosPID : public COREMotionController, public CORENamedObject {
    public:
        COREPosPID(double kP, double kI = 0, double kD = 0, double kF = 1);
        void setKp(double kP);
        void setKi(double kI);
        void setKd(double kD);
        double calculate();
    private:
        double m_kP, m_kI, m_kD, m_kF;
        double m_riemannSum, m_mistake, m_lastMistake;
        double m_proportional, m_integral, m_derivative;
    public:
        double getProportional() const;
        double getIntegral() const;
        double getDerivative() const;
        double getMistake();
    private:
        CORETimer timer;
    };
}