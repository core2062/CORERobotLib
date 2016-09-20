#ifndef SRC_CORELIBRARY_COREHARDWARE_COREJOYSTICK_H_
#define SRC_CORELIBRARY_COREHARDWARE_COREJOYSTICK_H_
#include <vector>
#include <map>
#include "../CORESubsystemsManager.h"


#ifdef __arm__
#include "WPILib.h"
#endif

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
class COREJoystick : public CORETask {
public:
	COREJoystick(unsigned int joystickNumber);
	void registerAxis(JoystickAxis axis);
	void registerButton(JoystickButton button);
	double getAxis(JoystickAxis axis);
	bool getButton(JoystickButton button);
	ButtonState getButtonState(JoystickButton button);
	void preTeleopTask();
private:
	Joystick joystick;
	map<JoystickButton, ButtonState> lastButtonCache, buttonCache;
	map<JoystickAxis, double> axisCache;
};

}
#endif
