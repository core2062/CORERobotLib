#include "COREJoystick.h"
#include "../COREHardware.h"

using namespace CORE;

COREJoystick::COREJoystick(int port) :
        m_joystick(port),
        m_joystickPort(port) {
}

void COREJoystick::registerAxis(JoystickAxis axis) {
    m_axisCache[axis] = m_joystick.GetRawAxis(axis);
}

void COREJoystick::registerVector(JoystickAxis axisA, JoystickAxis axisB){
	registerAxis(axisA);
	registerAxis(axisB);
}

void COREJoystick::registerButton(JoystickButton button) {
    m_buttonCache[button] = m_joystick.GetRawButton(button) ? ON : OFF;
}

double COREJoystick::getAxis(JoystickAxis axis) {
    if (m_axisCache.find(axis) == m_axisCache.end()) {
        registerAxis(axis);
        return m_joystick.GetRawAxis(axis);
    } else {
        return m_axisCache[axis];
    }
}

void COREJoystick::setAxis(JoystickAxis axis, int value) {
    m_axisCache[axis] = value;
}

Vector COREJoystick::getVector(JoystickAxis axisA, JoystickAxis axisB){
	Vector output;
	output.x = getAxis(axisA);
	output.y = -getAxis(axisB);
	return output;
}

bool COREJoystick::getButton(JoystickButton button) {
    if (m_buttonCache.find(button) == m_buttonCache.end()) {
        registerButton(button);
        return m_joystick.GetRawButton(button);
    } else {
        return (m_buttonCache[button] == RISING_EDGE || m_buttonCache[button] == ON);
    }
}

void COREJoystick::setButton(JoystickButton button, bool value) {
    m_buttonCache[button] = (value ? ON : OFF);
}

bool COREJoystick::getRisingEdge(JoystickButton button) {
    return m_buttonCache[button] == RISING_EDGE;
}

bool COREJoystick::getFallingEdge(JoystickButton button) {
    return m_buttonCache[button] == FALLING_EDGE;
}

COREJoystick::ButtonState COREJoystick::getButtonState(JoystickButton button) {
    if (m_buttonCache.find(button) == m_buttonCache.end()) {
        CORELog::logWarning("Joystick " + to_string(getPort()) + " button " + to_string(button)
                            + " not registered, registering and returning current state");
        registerButton(button);
        return m_joystick.GetRawButton(button) ? ON : OFF;
    } else {
        return m_buttonCache[button];
    }
}

int COREJoystick::getPort() {
    return m_joystickPort;
}

void COREJoystick::preLoopTask() {
    m_lastButtonCache = m_buttonCache;
    for (auto button : m_buttonCache) {
        bool isActive = m_joystick.GetRawButton(button.first);
        if (m_lastButtonCache[button.first] == FALLING_EDGE || m_lastButtonCache[button.first] == OFF) {
            if (isActive)
                m_buttonCache[button.first] = RISING_EDGE;
            else
                m_buttonCache[button.first] = OFF;
        } else if (m_lastButtonCache[button.first] == RISING_EDGE || m_lastButtonCache[button.first] == ON) {
            if (isActive)
                m_buttonCache[button.first] = ON;
            else
                m_buttonCache[button.first] = FALLING_EDGE;
        }
    }
    for (auto axis : m_axisCache) {
        m_axisCache[axis.first] = m_joystick.GetRawAxis(axis.first);
    }
}
