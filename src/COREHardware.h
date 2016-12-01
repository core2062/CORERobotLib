#pragma once

#include <map>
#include <memory>

#include "COREHardware/CORETimer.h"
#include "COREHardware/COREMotor.h"
#include "COREHardware/COREJoystick.h"
#include "WPILib.h"

using namespace CORE;
using namespace std;

namespace CORE {
    class Robot {
    public:
        static void addMotor(shared_ptr<COREMotor> motor);
        static void addJoystick(shared_ptr<COREJoystick> joystick);
        static shared_ptr<COREMotor> motor(int port);
        static shared_ptr<COREJoystick> joystick(int port);
        static void disableAllMotors();
    private:
        static map<int, shared_ptr<COREMotor>> m_motorMap;
        static map<int, shared_ptr<COREJoystick>> m_joystickMap;
    };
}