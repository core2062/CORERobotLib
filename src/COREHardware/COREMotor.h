#pragma once

#include <vector>
#include <memory>

#include "../COREScheduler.h"
#include "../COREPID.h"
#include "COREEncoder.h"

#include "WPILib.h"

namespace CORE {
    enum controlMode {
        VOLTAGE,
        PERCENTAGE,
        CURRENT,
        POS_PID,
        VEL_PID,
        POSVELPID
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
    class COREMotor : public COREPID, public COREEncoder {
    public:
        shared_ptr<CANTalon> CANTalonController;
        shared_ptr<Jaguar> JaguarController;
        shared_ptr<Victor> VictorController;
        COREMotor(int port, controllerType controller = CANTALON, encoderType encoder = NONE, controlMode controlMethod = VOLTAGE);
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
        void addSlave(shared_ptr<COREMotor> slaveMotor);
        void motorSafety(bool disableMotorSafety = true);
        void postTeleopTask();
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
        vector<COREMotor *> m_slaveMotors;
        shared_ptr<COREMotor> m_instance;
    };
}