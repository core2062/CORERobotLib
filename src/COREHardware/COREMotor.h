#pragma once

#include <vector>
#include <memory>
#include <iosfwd>

#include "COREFramework/CORETask.h"
#include "COREFramework/COREHardwareManager.h"
#include "COREUtilities/CORENamedObject.h"
#include "COREControl/COREPID.h"
#include "COREControl/COREController.h"
#include "COREEncoder.h"

#ifndef NOT_REAL
#include <WPILib.h>
#include <CANTalon.h>
#endif

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
    class COREMotor : public ICOREMotor, public ControllerOutput, public CORENamedObject {
    public:
        COREMotor(int port, controllerType controller = CANTALON, controlMode controlMethod = PERCENTAGE);
        void Set(double motorSetValue);
        double Get();
        double GetLast();
        void setReversed(bool reverse = true);
        bool getReversed();
        void setControlMode(controlMode controlMethod);
        controlMode getControlMode();
        int getPort() override;
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
        void Update() override;
    private:
        shared_ptr<CANTalon> m_CANTalonController = nullptr;
        shared_ptr<Jaguar> m_JaguarController = nullptr;
        shared_ptr<Victor> m_VictorController = nullptr;
        shared_ptr<COREEncoder> m_encoder = nullptr;
        double m_motorValue = 0;
        double m_lastMotorValue = 0;
        bool m_isFollower = false;
        bool m_reversed = false;
        double m_deadBandMin = 0;
        double m_deadBandMax = 0;
        bool m_motorUpdated = false;
        controlMode m_motorControlMode;
        controllerType m_motorControllerType;
        int m_motorPort;
        int m_motorSafetyCounter = 0;
        bool m_motorSafetyDisabled = false;
    };
}
