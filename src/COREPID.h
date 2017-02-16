#pragma once

#include <vector>
#include <memory>
#include <cmath>

#include "COREHardware/CORETimer.h"
#include <CORETask.h>
#include "COREMath.h"
#include "CORELog.h"
#include <COREController.h>
#include "COREHardware/CORESensor.h"

namespace CORE {
    enum PIDType {
        POS,
        VEL,
        ANG,
        POS_VEL,
        ANG_VEL
    };

    class COREPID : public CORETask, public COREContinuous {
    public:
        COREPID(ControllerInput* inputDevice, ControllerOutput* outputDevice, PIDType pidType, double pProfile1Value,
                double iProfile1Value, double dProfile1Value, double fProfile1Value = 1, double pProfile2Value = 0,
                double iProfile2Value = 0, double dProfile2Value = 1, double fProfile2Value = 0,
                int integralAccuracy = 1);
        COREPID(double pProfile1Value, double iProfile1Value, double dProfile1Value, double fProfile1Value = 1,
                double pProfile2Value = 0, double iProfile2Value = 0, double dProfile2Value = 1,
                double fProfile2Value = 0, int integralAccuracy = 1);
        double calculate(int profile = -1);
        void setPos(double positionSetPoint);
        void setVel(double velocitySetPoint);
        void setAng(double angleSetPoint);
        double getPos();
        double getVel();
        double getAng();
        void setActualPos(double actualPos);
        void setActualVel(double actualVel);
        void setActualAng(double actualAng);
        void setDefaultProfile(int profile);
        void setP(double value, PIDType pidType, int profile = -1);
        void setI(double value, PIDType pidType, int profile = -1);
        void setD(double value, PIDType pidType, int profile = -1);
        void setF(double value, PIDType pidType, int profile = -1);
        double getP(PIDType pidType, int profile = -1);
        double getI(PIDType pidType, int profile = -1);
        double getD(PIDType pidType, int profile = -1);
        double getF(PIDType pidType, int profile = -1);
        double getOutput(PIDType pidType, int profile = -1);
        double getProportional(PIDType pidType, int profile = -1);
        double getIntegral(PIDType pidType, int profile = -1);
        double getDerivative(PIDType pidType, int profile = -1);
        void preLoopTask() override;
        void postLoopTask() override;
    private:
        struct PIDProfile {
            struct PIDMode {
                double P, I, D, F, proportional, integral, derivative, output, lastOutput, mistake, lastMistake;
                vector<double> riemannSum;
            } pos, vel, ang;
        } m_PID1, m_PID2;
        struct {
            double setPoint, actual;
            bool enabled;
        } m_pos, m_vel, m_ang;
        PIDProfile* m_currentProfile;
        double m_ticksToDegrees = 1;
        double m_time = 0;
        int m_defaultProfile = 1;
        CORETimer m_timer;
        ControllerInput* m_inputDevice;
        ControllerOutput* m_outputDevice;
        PIDProfile::PIDMode* getPIDMode(PIDType pidType, int profile = -1);
        PIDProfile* getPIDProfile(int profile = -1);
        PIDType m_PIDTypes[3] = {POS, VEL, ANG};
        PIDType m_pidType;
        double posPID(double setPoint);
        double velPID(double setPoint);
        double angPID(double setPoint);
    };
}
