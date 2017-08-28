#pragma once

#include "COREUtilities/CORETimer.h"
#include "COREUtilities/CORENamedObject.h"
#include "COREUtilities/Rotation2d.h"
#include "COREController.h"

namespace CORE {
    class COREPID : public CORENamedObject, public COREMotionController {
    public:
        struct PIDProfile {
            double kP, kI, kD, kF;
            PIDProfile() = default;
            PIDProfile(double kProportional, double kIntegral, double kDerivative, double kFeedForward = 1) {
                kP = kProportional;
                kI = kIntegral;
                kD = kDerivative;
                kF = kFeedForward;
            }
        };
        explicit COREPID(double kP, double kI = 0, double kD = 0, double kF = 1);
        explicit COREPID(PIDProfile &profile);
        void setProportionalConstant(double kP);
        void setIntegralConstant(double kI);
        void setDerivativeConstant(double kD);
        void setFeedForwardConstant(double kF);
        void setPIDProfile(PIDProfile &profile);
        virtual double calculate(double mistake);
        virtual double calculate(double mistake, double dt);
        double getProportional() const;
        double getIntegral() const;
        double getDerivative() const;
        double getMistake() const;
    private:
        PIDProfile m_profile;
        CORETimer m_timer;
        double m_riemannSum, m_mistake, m_lastMistake;
        double m_proportional, m_integral, m_derivative;
    };

    class COREPositionPID : public COREPID {
    public:
        explicit COREPositionPID(double kP, double kI, double kD, double kF = 1);
        explicit COREPositionPID(PIDProfile &profile);
        double calculate(double actualPosition, double setPointPosition);
    };

    class COREAnglePID : public COREPID {
    public:
        explicit COREAnglePID(double kP, double kI, double kD, double kF = 1);
        explicit COREAnglePID(PIDProfile &profile);
        double calculate(Rotation2d actualAngle, Rotation2d setPointAngle);
    };
}
