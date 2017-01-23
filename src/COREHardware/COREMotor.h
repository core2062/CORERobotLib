#pragma once

#include <vector>
#include <memory>

#include "../COREScheduler.h"
#include "../COREPID.h"
#include "COREEncoder.h"

#include <WPILib.h>

namespace CORE {
    enum controlMode {
        VOLTAGE,
        PERCENTAGE,
        CURRENT,
        PID
    };

    enum controllerType {
        CANTALON,
        JAGUAR,
        VICTOR
    };

    enum limitSwitch {
        FORWARD,
        BACKWARD
    };
    class COREMotor : public PIDInput, public PIDOutput /*, public COREEncoder*/ {
    public:
        shared_ptr<CANTalon> CANTalonController;
        shared_ptr<Jaguar> JaguarController;
        shared_ptr<Victor> VictorController;
        COREMotor(int port, controllerType controller = CANTALON, controlMode controlMethod = VOLTAGE);
        void Set(double motorSetValue);
        double Get();
        void setReversed(bool reverse = true);
        bool getReversed();
        void setControlMode(controlMode controlMethod);
        controlMode getControlMode();
        int getPort();
        controllerType getControllerType();
        void setDeadband(double range);
        void setDeadband(double min, double max);
        void motorSafety(bool disableMotorSafety = true);
        void update();
        double PIDGetPos() override;
        double PIDGetVel() override;
        double PIDGetAng() override;
        void PIDSet(double value) override;
    private:
        double m_motorValue = 0;
        double m_lastMotorValue = 0;
        bool m_reversed = false;
        double m_trapSum = 0;
        double m_deadBandMin = 0;
        double m_deadBandMax = 0;
        bool m_motorUpdated = false;
        controlMode m_motorControlMode;
        controllerType m_motorControllerType;
        CORETimer *m_trapSumTimer;
        double m_lastSum = 0;
        int m_motorPort;
        int m_motorSafetyCounter = 0;
        bool m_motorSafetyDisabled = false;
    };
}
