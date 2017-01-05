#pragma once

#include <map>
#include <memory>

#include "COREHardware/CORETimer.h"
#include "COREHardware/COREMotor.h"
#include "COREHardware/COREEncoder.h"
#include "COREHardware/COREJoystick.h"
#include "COREHardware/CORESensor.h"
#include "WPILib.h"

using namespace CORE;
using namespace std;

namespace CORE {
    class Robot {
    public:
        static void addMotor(COREMotor* motor);
        static void addJoystick(COREJoystick* joystick);
        static COREMotor* motor(int port);
        static COREJoystick* joystick(int port);
        static void disableAllMotors();
    private:
        static map<int, COREMotor*> m_motorMap;
        static map<int, COREJoystick*> m_joystickMap;
    };
}
