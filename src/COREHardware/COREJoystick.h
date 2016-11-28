#pragma once

#include <vector>
#include <map>
#include "../COREScheduler.h"

#include "WPILib.h"

using namespace std;
using namespace CORE;

namespace CORE {
    enum JoystickAxis {
        LEFT_STICK_X,
        LEFT_STICK_Y,
        RIGHT_STICK_X,
        RIGHT_STICK_Y,
        RIGHT_TRIGGER_AXIS,
        LEFT_TRIGGER_AXIS
    };
    enum JoystickButton {
        DPAD_N,
        DPAD_NE,
        DPAD_E,
        DPAD_SE,
        DPAD_S,
        DPAD_SW,
        DPAD_W,
        DPAD_NW,
        LEFT_STICK,
        RIGHT_STICK,
        BACK_BUTTON,
        LOGO_BUTTON,
        START_BUTTON,
        Y_BUTTON,
        B_BUTTON,
        A_BUTTON,
        X_BUTTON,
        LEFT_BUTTON,
        RIGHT_BUTTON,
        LEFT_TRIGGER,
        RIGHT_TRIGGER
    };
    enum ButtonState {
        PRESSED,
        ACTIVE,
        RELEASED,
        NORMAL
    };
    enum JoystickType {
        F310
    };

    class COREJoystick : public CORETask {
    public:
        COREJoystick(uint32_t joystickNumber);
        void registerAxis(JoystickAxis axis);
        void registerButton(JoystickButton button);
        double getAxis(JoystickAxis axis);
        bool getButton(JoystickButton button);
        ButtonState getButtonState(JoystickButton button);
        int getPort();
        void preTeleopTask();
    private:
        Joystick m_joystick;
        uint32_t m_joystickPort;
        map<JoystickButton, ButtonState> m_lastButtonCache, m_buttonCache;
        map<JoystickAxis, double> m_axisCache;
        JoystickType m_expectedJoystickType;
        std::shared_ptr<COREJoystick> m_instance;
    };

}