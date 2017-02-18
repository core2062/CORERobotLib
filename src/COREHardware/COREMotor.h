#pragma once

#include <vector>
#include <memory>

#include "../CORETask.h"
#include "../COREPID.h"
#include "../COREController.h"
#include "COREEncoder.h"

#include <WPILib.h>
#include <CANTalon.h>

using namespace std;

namespace CORE {
    enum controlMode {
        VOLTAGE,
        PERCENTAGE,
        CURRENT
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
        COREMotor(int port, controllerType controller = CANTALON, controlMode controlMethod = VOLTAGE);
        void Set(double motorSetValue);
        double Get();
        void setReversed(bool reverse = true);
        bool getReversed();
        void setControlMode(controlMode controlMethod);
        controlMode getControlMode();
        int getPort();
        string getName();
        controllerType getControllerType();
        void setDeadband(double range);
        void setDeadband(double min, double max);
        void setFollower(int port);
        void motorSafety(bool disableMotorSafety = true);
        double getCurrent();
        shared_ptr<CANTalon> getCANTalon();
        shared_ptr<Jaguar> getJaguar();
        shared_ptr<Victor> getVictor();
        shared_ptr<COREEncoder> getEncoder();
        void ControllerSet(double value) override;
        void Update();
    private:
        shared_ptr<CANTalon> m_CANTalonController;
        shared_ptr<Jaguar> m_JaguarController;
        shared_ptr<Victor> m_VictorController;
        shared_ptr<COREEncoder> m_encoder;
        double m_motorValue = 0;
        double m_lastMotorValue = 0;
        bool m_isFollower = false;
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
