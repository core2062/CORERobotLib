#include "COREJoystick.h"

using namespace CORE;

/*
 * Initialize joystick with given port starting at 0
 */
COREJoystick::COREJoystick(int port) :
        m_joystick(port),
        m_joystickPort(port) {
}

/*
 * Register a joystick axis to be used. Must be called before a joystick axis can be used.
 */
void COREJoystick::registerAxis(JoystickAxis axis) {
    m_axisCache[axis] = m_joystick.GetRawAxis(axis);
}

/*
 * Register a joystick axis to be used. Must be called before a joystick vector can be used.
 */
void COREJoystick::registerVector(JoystickAxis axisA, JoystickAxis axisB) {
    registerAxis(axisA);
    registerAxis(axisB);
}

/*
 * Register a joystick button to be used. Must be called before a joystick button can be used.
 */
void COREJoystick::registerButton(JoystickButton button) {
    m_buttonCache[button] = m_joystick.GetRawButton(button) ? ON : OFF;
}

/*
 * Get an axis from the cache
 */
double COREJoystick::getAxis(JoystickAxis axis) {
    if(m_axisCache.find(axis) == m_axisCache.end()) {
        registerAxis(axis);
        return m_joystick.GetRawAxis(axis);
    } else {
        return m_axisCache[axis];
    }
}

/*
 * Set an axis manually to a given value, from -1 to 1
 */
void COREJoystick::setAxis(JoystickAxis axis, int value) {
    m_axisCache[axis] = value;
}

/*
 * Get a vector of two axises from the cache
 */
Vector COREJoystick::getVector(JoystickAxis axisA, JoystickAxis axisB) {
    Vector output;
    output.x = getAxis(axisA);
    output.y = -getAxis(axisB);
    return output;
}

/*
 * Get if a given button is pressed
 */
bool COREJoystick::getButton(JoystickButton button) {
    if(m_buttonCache.find(button) == m_buttonCache.end()) {
        registerButton(button);
        return m_joystick.GetRawButton(button);
    } else {
        return (m_buttonCache[button] == RISING_EDGE || m_buttonCache[button] == ON);
    }
}

/*
 * Set an button manually to a given value
 */
void COREJoystick::setButton(JoystickButton button, bool value) {
    m_buttonCache[button] = (value ? ON : OFF);
}

/*
 * Get if a given button is on its rising edge.
 * Rising edge means that last time the button was checked it was not pressed but now it is pressed
 */
bool COREJoystick::getRisingEdge(JoystickButton button) {
    return m_buttonCache[button] == RISING_EDGE;
}

/*
 * Get if a given button is on its falling edge.
 * Falling edge means that last time the button was checked it was pressed but now it is not pressed
 */
bool COREJoystick::getFallingEdge(JoystickButton button) {
    return m_buttonCache[button] == FALLING_EDGE;
}

/*
 * Get a given button's state
 */
COREJoystick::ButtonState COREJoystick::getButtonState(JoystickButton button) {
    if(m_buttonCache.find(button) == m_buttonCache.end()) {
        CORELog::logWarning("Joystick " + to_string(getPort()) + " button " + to_string(button)
                            + " not registered, registering and returning current state");
        registerButton(button);
        return m_joystick.GetRawButton(button) ? ON : OFF;
    } else {
        return m_buttonCache[button];
    }
}

/*
 * Get this joystick's port
 */
int COREJoystick::getPort() {
    return m_joystickPort;
}

/*
 * Update button cache before user code is run
 */
void COREJoystick::preLoopTask() {
    m_lastButtonCache = m_buttonCache;
    for(auto button : m_buttonCache) {
        bool isActive = m_joystick.GetRawButton(button.first);
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
        m_axisCache[axis.first] = m_joystick.GetRawAxis(axis.first);
    }
}
