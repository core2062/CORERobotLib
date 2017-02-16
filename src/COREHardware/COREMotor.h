#pragma once

#include <vector>
#include <memory>

#include "../CORETask.h"
#include "../COREPID.h"
#include "../COREController.h"
#include "COREEncoder.h"

#include <WPILib.h>
#include <CANTalon.h>

namespace CORE {
    enum controlMode {
        VOLTAGE,
        PERCENTAGE,
        CURRENT,
        FOLLOWER
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
    class COREMotor : public ControllerOutput {
    public:
        shared_ptr<CANTalon> CANTalonController;
        shared_ptr<Jaguar> JaguarController;
        shared_ptr<Victor> VictorController;
        shared_ptr<COREEncoder> Encoder;
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
        double getCurrent();
        void Update();
        void ControllerSet(double value) override;
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
        //CORETimer* m_trapSumTimer;
        //double m_lastSum = 0;
        int m_motorPort;
        int m_motorSafetyCounter = 0;
        bool m_motorSafetyDisabled = false;
    };
}
