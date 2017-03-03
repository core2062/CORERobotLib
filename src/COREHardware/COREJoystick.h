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
            RIGHT_STICK_X = 2,
            RIGHT_STICK_Y = 3,
            RIGHT_TRIGGER_AXIS = 4,
            LEFT_TRIGGER_AXIS = 5
        };
        enum JoystickButton {
            DPAD_N = 6,
            DPAD_NE = 7,
            DPAD_E = 8,
            DPAD_SE = 9,
            DPAD_S = 10,
            DPAD_SW = 11,
            DPAD_W = 12,
            DPAD_NW = 13,
            LEFT_STICK = 14,
            RIGHT_STICK = 15,
            BACK_BUTTON = 16,
            START_BUTTON = 17,
            Y_BUTTON = 18,
            B_BUTTON = 19,
            A_BUTTON = 20,
            X_BUTTON = 21,
            LEFT_BUTTON = 22,
            RIGHT_BUTTON = 23,
            LEFT_TRIGGER = 24,
            RIGHT_TRIGGER = 25
        };
        enum ButtonState {
            RISING_EDGE,
            ON,
            FALLING_EDGE,
            OFF
        };
        enum JoystickType {
            F310_X_MODE,
			F310_D_MODE
        };

        COREJoystick(int joystickNumber, JoystickType expectedJoystickType = F310_X_MODE);
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
        int m_F310_X_Mode_Map[26] = {
                0, //LEFT_STICK_X
				1, //LEFT_STICK_Y
				4, //RIGHT_STICK_X
				5, //RIGHT_STICK_Y
				3, //RIGHT_TRIGGER_AXIS
				2, //LEFT_TRIGGER_AXIS
				-8, //DPAD_N
				-7, //DPAD_NE
				-6, //DPAD_E
				-5, //DPAD_SE
				-4, //DPAD_S
				-3, //DPAD_SW
				-2, //DPAD_W
				-1, //DPAD_NW
				9, //LEFT_STICK
				10, //RIGHT_STICK
				7, //BACK_BUTTON
				8, //START_BUTTON
				4, //Y_BUTTON
                2, //B_BUTTON
                1, //A_BUTTON
                3, //X_BUTTON
                5, //LEFT_BUTTON
                6, //RIGHT_BUTTON
                0, //LEFT_TRIGGER
                0  //RIGHT_TRIGGER
        };

        int m_F310_D_Mode_Map[26] = {
				0, //LEFT_STICK_X
				1, //LEFT_STICK_Y
				4, //RIGHT_STICK_X
				5, //RIGHT_STICK_Y
				3, //RIGHT_TRIGGER_AXIS
				2, //LEFT_TRIGGER_AXIS
				-8, //DPAD_N
				-7, //DPAD_NE
				-6, //DPAD_E
				-5, //DPAD_SE
				-4, //DPAD_S
				-3, //DPAD_SW
				-2, //DPAD_W
				-1, //DPAD_NW
				9, //LEFT_STICK
				10, //RIGHT_STICK
				7, //BACK_BUTTON
				8, //START_BUTTON
				4, //Y_BUTTON
				2, //B_BUTTON
				1, //A_BUTTON
				3, //X_BUTTON
				5, //LEFT_BUTTON
				6, //RIGHT_BUTTON
				0, //LEFT_TRIGGER
				0  //RIGHT_TRIGGER
		};
        Joystick m_joystick;
        int m_joystickPort;
        map<JoystickButton, ButtonState> m_lastButtonCache, m_buttonCache;
        map<JoystickAxis, double> m_axisCache;
        JoystickType m_expectedJoystickType;
        std::shared_ptr<COREJoystick> m_instance;
        bool getPOVButton(JoystickButton button);
    };

}
