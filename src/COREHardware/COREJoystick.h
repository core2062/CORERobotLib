#pragma once

#include <vector>
#include <map>
#include "../CORETask.h"
#include "../CORELog.h"
#include "../COREMath.h"

#include <WPILib.h>

using namespace std;
using namespace CORE;

namespace CORE {
    class COREJoystick : public CORETask {
    public:
        enum JoystickAxis {
            LEFT_STICK_X = 0,
            LEFT_STICK_Y = 1,
            RIGHT_STICK_X = 4,
            RIGHT_STICK_Y = 5,
            RIGHT_TRIGGER_AXIS = 3,
            LEFT_TRIGGER_AXIS = 2
        };
        enum JoystickButton {
            DPAD_N=-8,
            DPAD_NE=-7,
            DPAD_E=-6,
            DPAD_SE=-5,
            DPAD_S=-4,
            DPAD_SW=-3,
            DPAD_W=-2,
            DPAD_NW=-1,
            LEFT_STICK = 9,
            RIGHT_STICK = 10,
            BACK_BUTTON = 7,
            LOGO_BUTTON,
            START_BUTTON = 8,
            Y_BUTTON = 4,
            B_BUTTON = 2,
            A_BUTTON = 1,
            X_BUTTON = 3,
            LEFT_BUTTON = 5,
            RIGHT_BUTTON = 6,
            LEFT_TRIGGER,
            RIGHT_TRIGGER
        };
        enum ButtonState {
            RISING_EDGE,
            ON,
            FALLING_EDGE,
            OFF
        };
        enum JoystickType {
            F310
        };

        COREJoystick(int joystickNumber);
        void registerAxis(JoystickAxis axis);
        void registerVector(JoystickAxis axisA, JoystickAxis axisB);
        void registerButton(JoystickButton button);
        double getAxis(JoystickAxis axis);
        void setAxis(JoystickAxis axis, int value);
        Vector getVector(JoystickAxis axisA, JoystickAxis axisB);
        bool getButton(JoystickButton button);
        void setButton(JoystickButton button, bool value);
        bool getRisingEdge(JoystickButton button);
        bool getFallingEdge(JoystickButton button);
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
        bool getPOVButton(JoystickButton button);
    };

}
