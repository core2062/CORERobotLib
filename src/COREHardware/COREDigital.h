#pragma once

#include <memory>
#include "WPILib.h"

using namespace std;

namespace CORE {
    class COREMotor;
    enum limitSwitch {
        FORWARD,
        BACKWARD
    };
    class COREDigital {
    public:
        COREDigital(shared_ptr<COREMotor> motor);
        void Set(bool value);
        bool Get();
    private:
        shared_ptr<COREMotor> m_motor = nullptr;
        shared_ptr<DigitalInput> m_digitalInput;
        limitSwitch m_limitSwitchPosition;
    };
}