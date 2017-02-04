#pragma once

#include <vector>
#include <map>
#include <CORETask.h>
#include "../COREMath.h"

#include <WPILib.h>

using namespace std;
using namespace CORE;

namespace CORE {
    enum JoystickAxis {
        LEFT_STICK_X = 0,
        LEFT_STICK_Y = 1,
        RIGHT_STICK_X=4,
        RIGHT_STICK_Y=5,
        RIGHT_TRIGGER_AXIS=3,
        LEFT_TRIGGER_AXIS = 2
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
        COREJoystick(int joystickNumber);
        void registerAxis(JoystickAxis axis);
        void registerVector(JoystickAxis axisA, JoystickAxis axisB);
        void registerButton(JoystickButton button);
        double getAxis(JoystickAxis axis);
        void setAxis(JoystickAxis axis, int value);
        Vector getVector(JoystickAxis axisA, JoystickAxis axisB);
        bool getButton(JoystickButton button);
        void setButton(JoystickButton button, bool value);
        ButtonState getButtonState(JoystickButton button);
        int getPort();
        void preLoopTask();
    private:
        Joystick m_joystick;
        int m_joystickPort;
        map<JoystickButton, ButtonState> m_lastButtonCache, m_buttonCache;
        map<JoystickAxis, double> m_axisCache;
        JoystickType m_expectedJoystickType;
        std::shared_ptr<COREJoystick> m_instance;
    };

}
