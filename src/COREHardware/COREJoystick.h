#pragma once

#include <vector>
#include <map>
#include "../CORESubsystemsManager.h"

#include "WPILib.h"

using namespace std;
using namespace CORE;

namespace CORE {
enum JoystickAxis {
	LEFTSTICKX,
	LEFTSTICKY,
	RIGHTSTICKX,
	RIGHTSTICKY,
	RIGHTTRIGGERAXIS,
	LEFTTRIGGERAXIS
};
enum JoystickButton {
	DPADN,
	DPADNE,
	DPADE,
	DPADSE,
	DPADS,
	DPADSW,
	DPADW,
	DPADNW,
	LEFTSTICK,
	RIGHTSTICK,
	BACKBUTTON,
	LOGOBUTTON,
	STARTBUTTON,
	YBUTTON,
	BBUTTON,
	ABUTTON,
	XBUTTON,
	LEFTBUTTON,
	RIGHTBUTTON,
	LEFTTRIGGER,
	RIGHTTRIGGER
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
	Joystick joystick;
	uint32_t joystickPort;
	map<JoystickButton, ButtonState> lastButtonCache, buttonCache;
	map<JoystickAxis, double> axisCache;
	JoystickType expectedJoystickType;
	std::shared_ptr<COREJoystick> instance;
};

}