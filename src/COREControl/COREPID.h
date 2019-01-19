#pragma once

#include "COREUtilities/CORETimer.h"
#include "COREUtilities/CORENamedObject.h"
#include "COREUtilities/COREVector.h"
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
        void SetProportionalConstant(double kP);
        void SetIntegralConstant(double kI);
        void SetDerivativeConstant(double kD);
        void SetFeedForwardConstant(double kF);
        double GetProportionalConstant();
        double GetIntegralConstant();
        double GetDerivativeConstant();
        double GetFeedForwardConstant();
        void SetPIDProfile(PIDProfile &profile);
        virtual double Calculate(double mistake);
        virtual double Calculate(double mistake, double dt);
        double GetProportional() const;
        double GetIntegral() const;
        double GetDerivative() const;
        double GetMistake() const;
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
        double Calculate(double actualPosition, double setPointPosition) override;
        double Calculate(double actualPosition, double setPointPosition, double dt);
    };

    class COREAnglePID : public COREPID {
    public:
        explicit COREAnglePID(double kP, double kI, double kD, double kF = 1);
        explicit COREAnglePID(PIDProfile &profile);
        double Calculate(COREVector actualAngle, COREVector setPointAngle);
        double Calculate(COREVector actualAngle, COREVector setPointAngle, double dt);
    };
}
