#include "COREJoystick.h"
#include "COREFramework/CORERobot.h"

using namespace CORE;

bool COREJoystick::GetPOVButton(JoystickButton button) {
	switch(m_joystick.GetPOV()) {
	case 0:
		return button == DPAD_N;
		break;
	case 45:
		return button == DPAD_NE;
		break;
	case 90:
		return button == DPAD_E;
		break;
	case 135:
		return button == DPAD_SE;
		break;
	case 180:
		return button == DPAD_S;
		break;
	case 225:
		return button == DPAD_SW;
		break;
	case 270:
		return button == DPAD_W;
		break;
	case 315:
		return button == DPAD_NW;
		break;
	default:
		return false;
		break;
	}
}

/*
 * Initialize joystick with given port starting at 0
 */
COREJoystick::COREJoystick(int port, JoystickType expectedJoystickType) :
        m_joystick(port),
        m_joystickPort(port),
		m_expectedJoystickType(expectedJoystickType) {
/*	string actualJoystickName = m_joystick.GetName();
	switch(m_expectedJoystickType) {
	case F310_X_MODE:
		if(actualJoystickName != "Controller (Gamepad F310)") {

		}

		break;
	case F310_D_MODE:

		break;
	}*/
}

/*
 * Register a joystick axis to be used. Must be called before a joystick axis can be used.
 */
void COREJoystick::RegisterAxis(JoystickAxis axis) {
    m_axisCache[axis] = m_joystick.GetRawAxis(axis);
}

/*
 * Register a joystick axis to be used. Must be called before a joystick vector can be used.
 */
void COREJoystick::RegisterVector(JoystickAxis axisA, JoystickAxis axisB) {
    RegisterAxis(axisA);
    RegisterAxis(axisB);
}

/*
 * Register a joystick button to be used. Must be called before a joystick button can be used.
 */
void COREJoystick::RegisterButton(JoystickButton button) {
    if (button == JoystickButton::RIGHT_TRIGGER) {
        m_buttonCache[button] = abs(m_joystick.GetRawAxis(JoystickAxis::RIGHT_TRIGGER_AXIS)) > 0 ? ON : OFF;
    } else if (button == JoystickButton::LEFT_TRIGGER) {
        m_buttonCache[button] = abs(m_joystick.GetRawAxis(JoystickAxis::LEFT_TRIGGER_AXIS)) > 0 ? ON : OFF;
    } else if(button > -1){
		m_buttonCache[button] = (m_joystick.GetRawButton(button)) ? ON : OFF;
	} else {
		m_buttonCache[button] = (GetPOVButton(button)) ? ON : OFF;
	}
}

/*
 * Get an axis from the cache
 */
double COREJoystick::GetAxis(JoystickAxis axis) {
    if(m_axisCache.find(axis) == m_axisCache.end()) {
        RegisterAxis(axis);
        return m_joystick.GetRawAxis(axis);
    } else {
        return m_axisCache[axis];
    }
}

/*
 * Set an axis manually to a given value, from -1 to 1
 */
void COREJoystick::SetAxis(JoystickAxis axis, int value) {
    m_axisCache[axis] = value;
}

/*
 * Get a vector of two axises from the cache
 */
Vector COREJoystick::GetVector(JoystickAxis axisA, JoystickAxis axisB) {
    Vector output;
    output.x = GetAxis(axisA);
    output.y = -GetAxis(axisB);
    return output;
}

/*
 * Get if a given button is pressed
 */
bool COREJoystick::GetButton(JoystickButton button) {
    if(m_buttonCache.find(button) == m_buttonCache.end()) {
        RegisterButton(button);
        return GetButton(button);
    } else {
        return (m_buttonCache[button] == RISING_EDGE || m_buttonCache[button] == ON);
    }
}

/*
 * Set an button manually to a given value
 */
void COREJoystick::SetButton(JoystickButton button, bool value) {
    m_buttonCache[button] = (value ? ON : OFF);
}

/*
 * Get if a given button is on its rising edge.
 * Rising edge means that last time the button was checked it was not pressed but now it is pressed
 */
bool COREJoystick::GetRisingEdge(JoystickButton button) {
    return m_buttonCache[button] == RISING_EDGE;
}

/*
 * Get if a given button is on its falling edge.
 * Falling edge means that last time the button was checked it was pressed but now it is not pressed
 */
bool COREJoystick::GetFallingEdge(JoystickButton button) {
    return m_buttonCache[button] == FALLING_EDGE;
}

/*
 * Get a given button's state
 */
COREJoystick::ButtonState COREJoystick::GetButtonState(JoystickButton button) {
    if(m_buttonCache.find(button) == m_buttonCache.end()) {
        CORELog::LogWarning("Joystick " + to_string(GetPort()) + " button " + to_string(button)
                            + " not registered, registering and returning current state");
        RegisterButton(button);
        return m_joystick.GetRawButton(button) ? ON : OFF;
    } else {
        return m_buttonCache[button];
    }
}

/*
 * Get this joystick's port
 */
int COREJoystick::GetPort() {
    return m_joystickPort;
}

/*
 * Update button cache before user code is run
 */
void COREJoystick::PreLoopTask() {
    m_lastButtonCache = m_buttonCache;
    for(auto button : m_buttonCache) {
        bool isActive;
        if(COREDriverstation::GetMode() == COREDriverstation::AUTON 
           || COREDriverstation::GetMode() == COREDriverstation::DISABLE) {
            m_buttonCache[button.first] = OFF;
            continue;
        }
        if (button.first == JoystickButton::RIGHT_TRIGGER) {
            isActive = abs(m_joystick.GetRawAxis(JoystickAxis::RIGHT_TRIGGER_AXIS)) > 0;
        } else if (button.first == JoystickButton::LEFT_TRIGGER) {
            isActive = abs(m_joystick.GetRawAxis(JoystickAxis::LEFT_TRIGGER_AXIS)) > 0;
        } else if (button.first > -1) {
            isActive = m_joystick.GetRawButton(button.first);
        } else {
    		isActive = GetPOVButton(button.first);
    	}
    	if(m_lastButtonCache[button.first] == FALLING_EDGE || m_lastButtonCache[button.first] == OFF) {
			if(isActive) {
				m_buttonCache[button.first] = RISING_EDGE;
			} else {
				m_buttonCache[button.first] = OFF;
			}
		} else if(m_lastButtonCache[button.first] == RISING_EDGE || m_lastButtonCache[button.first] == ON) {
			if(isActive) {
				m_buttonCache[button.first] = ON;
			} else {
				m_buttonCache[button.first] = FALLING_EDGE;
			}
		}
    }
    for(auto axis : m_axisCache) {
    	/*if(COREDriverstation::getMode() == COREDriverstation::AUTON //TODO: REENABLE THIS SAFETY FEATURE LATER
		   || COREDriverstation::getMode() == COREDriverstation::DISABLE) {
    		m_axisCache[axis.first] = 0;
			continue;
		}*/
        m_axisCache[axis.first] = m_joystick.GetRawAxis(axis.first);
    }
}
