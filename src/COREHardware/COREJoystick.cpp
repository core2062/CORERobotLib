#include "COREJoystick.h"
#include "../COREHardware.h"
#include <iostream>

#ifdef __arm__
#include "WPILib.h"
#endif

using namespace CORE;

COREJoystick::COREJoystick(int port) :
	joystick(port),
	joystickPort(port),
	instance(this)
{
	Robot::addJoystick(instance);
}

void COREJoystick::registerAxis(JoystickAxis axis) {
	axisCache[axis] = joystick.GetRawAxis(axis);
}

void COREJoystick::registerButton(JoystickButton button) {
	buttonCache[button] = joystick.GetRawButton(button)  ? ACTIVE : NORMAL;
}

double COREJoystick::getAxis(JoystickAxis axis) {
	if(axisCache.find(axis) == axisCache.end()) {
		registerAxis(axis);
		return joystick.GetRawAxis(axis);
	}
	else {
		return axisCache[axis];
	}
}

bool COREJoystick::getButton(JoystickButton button) {
	if(buttonCache.find(button) == buttonCache.end()) {
		registerButton(button);
		return joystick.GetRawButton(button);
	}
	else {
		return (buttonCache[button] == PRESSED || buttonCache[button] == ACTIVE);
	}
}

ButtonState COREJoystick::getButtonState(JoystickButton button) {
	if(buttonCache.find(button) == buttonCache.end()) {
		//TODO: Error: button not registered, registering and returning state
		registerButton(button);
		return joystick.GetRawButton(button) ? ACTIVE : NORMAL;
	}
	else {
		return buttonCache[button];
	}
}

int COREJoystick::getPort() {
	return joystickPort;
}

void COREJoystick::preTeleopTask() {
	lastButtonCache = buttonCache;
	for(auto button : buttonCache) {
		bool isActive = joystick.GetRawButton(button.first);
		if(lastButtonCache[button.first] == RELEASED || lastButtonCache[button.first] == NORMAL) {
			if(isActive)
				buttonCache[button.first] = PRESSED;
			else
				buttonCache[button.first] = NORMAL;
		}
		else if (lastButtonCache[button.first] == PRESSED || lastButtonCache[button.first] == ACTIVE) {
			if(isActive)
				buttonCache[button.first] = ACTIVE;
			else
				buttonCache[button.first] = RELEASED;
		}
	}
	for(auto axis : axisCache) {
		axisCache[axis.first] = joystick.GetRawAxis(axis.first);
	}
}
