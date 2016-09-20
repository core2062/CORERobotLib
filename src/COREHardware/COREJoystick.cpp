#include "COREJoystick.h"

using namespace CORE;

COREJoystick::COREJoystick(unsigned int joystickNumber) :
	joystick(joystickNumber)
{

}

void COREJoystick::registerAxis(JoystickAxis axis) {
	axisCache[axis] = joystick.GetRawAxis(axis);
}

void COREJoystick::registerButton(JoystickButton button) {
	buttonCache[button] = joystick.GetRawButton(button)  ? ACTIVE : NORMAL;
}

double COREJoystick::getAxis(JoystickAxis axis) {
	if(axisCache.find(axis) == axisCache.end()) {
		//TODO: Error: axis not registered, regestering and returing 0
		registerAxis(axis);
		return joystick.GetRawAxis(axis);
	}
	else {
		return axisCache[axis];
	}
}

bool COREJoystick::getButton(JoystickButton button) {
	if(buttonCache.find(button) == buttonCache.end()) {
		//TODO: Error: button not registered, regestering and returing state
		registerButton(button);
		return joystick.GetRawButton(button);
	}
	else {
		return (buttonCache[button] == PRESSED || buttonCache[button] == ACTIVE);
	}
}

ButtonState COREJoystick::getButtonState(JoystickButton button) {
	if(buttonCache.find(button) == buttonCache.end()) {
		//TODO: Error: button not registered, regestering and returing state
		registerButton(button);
		return joystick.GetRawButton(button) ? ACTIVE : NORMAL;
	}
	else {
		return buttonCache[button];
	}
}

void COREJoystick::preTeleopTask() {
	lastButtonCache = buttonCache;
	for(auto button : buttonCache) {
		bool isActive = joystick.GetRawButton(button.first);
		if(lastButtonCache[button.first] == RELEASED || lastButtonCache[button.first] == NORMAL) {
			if(isActive)
				button.second = PRESSED;
			else
				button.second = NORMAL;
		}
		else if (lastButtonCache[button.first] == PRESSED || lastButtonCache[button.first] == ACTIVE) {
			if(isActive)
				button.second = ACTIVE;
			else
				button.second = RELEASED;
		}
	}
	for(auto axis : axisCache) {
		axis.second = joystick.GetRawAxis(axis.first);
	}
}
