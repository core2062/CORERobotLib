#include "COREJoystick.h"
#include "../COREHardware.h"

using namespace CORE;

COREJoystick::COREJoystick(uint32_t port) :
        m_joystick(port),
        m_joystickPort(port),
        m_instance(this) {
    Robot::addJoystick(m_instance);
}

void COREJoystick::registerAxis(JoystickAxis axis) {
    m_axisCache[axis] = m_joystick.GetRawAxis(axis);
}

void COREJoystick::registerButton(JoystickButton button) {
    m_buttonCache[button] = m_joystick.GetRawButton(button) ? ACTIVE : NORMAL;
}

double COREJoystick::getAxis(JoystickAxis axis) {
    if (m_axisCache.find(axis) == m_axisCache.end()) {
        registerAxis(axis);
        return m_joystick.GetRawAxis(axis);
    } else {
        return m_axisCache[axis];
    }
}

bool COREJoystick::getButton(JoystickButton button) {
    if (m_buttonCache.find(button) == m_buttonCache.end()) {
        registerButton(button);
        return m_joystick.GetRawButton(button);
    } else {
        return (m_buttonCache[button] == PRESSED || m_buttonCache[button] == ACTIVE);
    }
}

ButtonState COREJoystick::getButtonState(JoystickButton button) {
    if (m_buttonCache.find(button) == m_buttonCache.end()) {
        //TODO: Error: button not registered, registering and returning state
        registerButton(button);
        return m_joystick.GetRawButton(button) ? ACTIVE : NORMAL;
    } else {
        return m_buttonCache[button];
    }
}

int COREJoystick::getPort() {
    return m_joystickPort;
}

void COREJoystick::preTeleopTask() {
    m_lastButtonCache = m_buttonCache;
    for (auto button : m_buttonCache) {
        bool isActive = m_joystick.GetRawButton(button.first);
        if (m_lastButtonCache[button.first] == RELEASED || m_lastButtonCache[button.first] == NORMAL) {
            if (isActive)
                m_buttonCache[button.first] = PRESSED;
            else
                m_buttonCache[button.first] = NORMAL;
        } else if (m_lastButtonCache[button.first] == PRESSED || m_lastButtonCache[button.first] == ACTIVE) {
            if (isActive)
                m_buttonCache[button.first] = ACTIVE;
            else
                m_buttonCache[button.first] = RELEASED;
        }
    }
    for (auto axis : m_axisCache) {
        m_axisCache[axis.first] = m_joystick.GetRawAxis(axis.first);
    }
}
